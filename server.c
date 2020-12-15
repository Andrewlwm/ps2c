#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "utils.h"

int main(void)
{
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    char reicv_line[BUFSIZE];
    char response_msg[BUFSIZE];
    int sin_size;
    int yes = 1;
    char *res_OK = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n\r\n";
    char *res_TO = "HTTP/1.1 408 REQUEST TIMEOUT\r\n"
                   "Connection: close\r\n\r\n";

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), 0, 8);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    {
        char path[] = "./public";
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                             &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }
        printf("server: conexiune de la: %s\n",
               inet_ntoa(their_addr.sin_addr));
        int n;
        memset(reicv_line, 0, BUFSIZE);
        if ((n = recv(new_fd, reicv_line, BUFSIZE - 1, 0)) > 0)
        {
            //printf("request_str: %s", reicv_line);
        }
        else
        {
            send(new_fd, (char *)res_TO, strlen((char *)res_TO), 0);
            close(new_fd);
            continue;
        }
        FILE *f = NULL;
        char *file_to_char = read_file(&f, file_to_char, get_path_from_req(reicv_line, path));

        memset(response_msg, 0, BUFSIZE);
        sprintf(response_msg, "%s%s%s", res_OK, file_to_char, "\r\n");

        send(new_fd, (char *)response_msg, strlen((char *)response_msg), 0);

        fclose(f);
        free(file_to_char);
        close(new_fd);
    }

    return 0;
}
