#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 5000
#define BUFSIZE 4096

int main(int argc, char *argv[])
{
    int sockfd, n;
    char reicv_line[BUFSIZE];
    char request_msg[BUFSIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;

    if (argc < 2)
    {
        fprintf(stderr, "utilizare: client hostname opt:filename\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&their_addr,
                sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    char *req_h = "GET /";
    char *req_t = "HTTP/1.1\r\n";
    memset(request_msg, 0, BUFSIZE);
    if (argv[2] != NULL)
        sprintf(request_msg, "%s%s %s", req_h, argv[2], req_t);
    else
        sprintf(request_msg, "%s %s", req_h, req_t);
    send(sockfd, (char *)request_msg, strlen((char *)request_msg), 0);

    if ((n = recv(sockfd, reicv_line, BUFSIZE - 1, 0)) > 0)
    {
        printf("response_str: %s", reicv_line);
    }
    else
    {
        perror("readresponse");
    }

    close(sockfd);

    return 0;
}
