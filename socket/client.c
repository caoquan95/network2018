#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

int main()
{
    int sockfd;
    struct hostent *h;
    struct sockaddr_in saddr;
    char buffer[1025];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error creating socket\n");
    }
    if ((h = gethostbyname("ec2-52-76-150-129.ap-southeast-1.compute.amazonaws.com")) == NULL)
    {
        printf("Unknown host\n");
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(8784);

    if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        printf("Cannot connect\n");
    }

    printf("Client: ");
    scanf("%s", buffer);

    write(sockfd, buffer, sizeof(buffer));

    read(sockfd, buffer, sizeof(buffer));

    printf("Server: %s\n", buffer);
}