#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/select.h>

int main()
{
    int sockfd;
    char buffer[1025];
    int clientfds[100];
    memset(clientfds, 0, sizeof(clientfds));

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
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        int maxfd = sockfd;

        for (int i = 0; i < 100; i++)
        {
            if (clientfds[i] > 0)
            {
                // printf("adding %d into set\n", clientfds[i]);
                FD_SET(clientfds[i], &set);
            }
            if (clientfds[i] > maxfd)
                maxfd = clientfds[i];
        }

        select(maxfd + 1, &set, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &set))
        {
            int clientfd = accept(sockfd, (struct sockaddr *)NULL, NULL);

            // make it non blocking
            int fl = fcntl(clientfd, F_GETFL, 0);
            fl |= O_NONBLOCK;
            fcntl(clientfd, F_SETFL, fl);

            // add it to the clientfds array
            for (int i = 0; i < 100; i++)
            {
                if (clientfds[i] == 0)
                {
                    clientfds[i] = clientfd;
                    break;
                }
            }
        }
        for (int i = 0; i < 100; i++)
        {
            if (clientfds[i] > 0 && FD_ISSET(clientfds[i], &set))
            {
                if (read(clientfds[i], buffer, sizeof(buffer)) > 0)
                {
                    printf("client %d says: %s\nserver>", clientfds[i], buffer);
                }
                else
                {
                    printf("client %d has disconnected.\n", clientfds[i]);
                    clientfds[i] = 0;
                }
            }
        }

        // while (1)
        // {
        //     if (read(clientfd, buffer, sizeof(buffer)) > 0)
        //         printf("Client: %s\n", buffer);

        //     printf("Server: ");
        //     scanf("%s", buffer);
        //     write(clientfd, buffer, sizeof(buffer));
        // }

        // close(clientfd);
    }
}