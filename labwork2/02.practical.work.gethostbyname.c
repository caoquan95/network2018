#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    char host_name[255];
    printf("Hostname please:\n");
    scanf("%s", host_name);
    struct hostent *host = gethostbyname(host_name);

    struct in_addr addr;
    memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
    printf("address: %s\n", inet_ntoa(addr));
}