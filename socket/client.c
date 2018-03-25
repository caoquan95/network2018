#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>

int sockfd;
char buffer[1025];

void *writeToServer()
{
    while (1)
    {

        if (sockfd > 0)
        {
            // printf("Send to server: \n");
            fgets(buffer, sizeof(buffer), stdin);
            write(sockfd, buffer, sizeof(buffer));
        }
    }
    return NULL;
}

int main()
{

    struct hostent *h;
    struct sockaddr_in saddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error creating socket\n");
    }

    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, fl);

    if ((h = gethostbyname("ec2-52-76-150-129.ap-southeast-1.compute.amazonaws.com")) == NULL)
    {
        printf("Unknown host\n");
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(8784);

    connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));

    pthread_t tid;
    pthread_create(&tid, NULL, writeToServer, NULL);

    while (1)
    {
        if (read(sockfd, buffer, sizeof(buffer)) > 0)
        {
            printf("%s", buffer);
        }
    }
}