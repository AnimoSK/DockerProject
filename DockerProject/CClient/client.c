#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PORT 9080

int main()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *plaintext;
    char *encryptedtext;
    encryptedtext = malloc(sizeof(char) * 17);
    plaintext = malloc(sizeof(char) * 17);
    printf("Enter plaintext: ");
    fgets(plaintext, 17, stdin);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Mention the Server Container Address in inet_addr function
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("172.17.0.2");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock, plaintext, strlen(plaintext), 0);
    printf("Plaintext sent to the server\n");

    valread = read(sock, encryptedtext, 17);
    printf("Encrypted Text: %s\n", encryptedtext);

    free(plaintext);
    free(encryptedtext);
    return 0;
}
