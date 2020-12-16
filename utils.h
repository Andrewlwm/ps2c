#ifndef __UTILS_H
#define __UTILS_H

#define PORT 5000
#define BACKLOG 10
#define BUFSIZE 4096

char *read_file(FILE **f, char *read_file, char *path);
char *get_path_from_req(char *req_body, char *path);

#endif //__UTILS_H