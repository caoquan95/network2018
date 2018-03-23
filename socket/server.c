#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int serverSocket;
    char buffer[1025];

    // create new socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("create socket failed");
        exit(0);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8784);

    // bind socket to port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("bind socket failed");
        exit(0);
    }

    listen(serverSocket, 10);

    while (1)
    {
        printf("Waiting for connection\n");
        int connection = accept(serverSocket, (struct sockaddr *)NULL, NULL);
        read(connection, buffer, sizeof(buffer));
        printf("Client: %s", buffer);

        printf("Server: \n");
        scanf("%s", buffer);

        write(connection, buffer, sizeof(buffer));

        close(connection);
        sleep(1);
    }
}