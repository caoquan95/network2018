#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main()
{
    int sockfd;
    char buffer[1025];

    // create new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("create socket failed");
        exit(0);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, fl);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8784);

    // bind socket to port
    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("bind socket failed");
        exit(0);
    }

    listen(sockfd, 10);

    while (1)
    {
        int clientfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
        if (clientfd > 0)
        {
            int fl = fcntl(clientfd, F_GETFL, 0);
            fl |= O_NONBLOCK;
            fcntl(clientfd, F_SETFL, fl);
            while (1)
            {
                if (read(clientfd, buffer, sizeof(buffer)) > 0)
                    printf("Client: %s\n", buffer);

                printf("Server: ");
                scanf("%s", buffer);
                write(clientfd, buffer, sizeof(buffer));
            }

            close(clientfd);
        }
    }
}