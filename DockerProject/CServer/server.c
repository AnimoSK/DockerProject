#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "encrypt.h"
#define PORT 9080

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *plaintext;
    char *encryptedtext;
    plaintext = malloc(sizeof(char) * 17);
    encryptedtext = malloc(sizeof(char) * 17);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, plaintext, 17);
    printf("Plaintext recieved: %s", plaintext);

    //AES Algorithm Implementation in encrypt.c
    encryptedtext = aesEncrypt(plaintext);

    send(new_socket, encryptedtext, strlen(encryptedtext), 0);
    printf("\nEncrypted text sent to the client\n");
    free(encryptedtext);
    free(plaintext);
    return 0;
}
