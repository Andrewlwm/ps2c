#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

char *read_file(FILE **f, char *read_file, char *path)
{
    read_file = NULL;
    unsigned long size = 0;

    if ((*f = fopen(path, "r")) == NULL)
    {
        *f = fopen("./public/404.html", "r");
    }

    fseek(*f, 0, SEEK_END);
    size = ftell(*f);

    rewind(*f);

    read_file = malloc((size + 1) * sizeof(read_file));
    fread(read_file, size, 1, *f);
    read_file[size] = 0;

    return read_file;
}

char *get_path_from_req(char *req_body, char *path)
{
    char *first_line;
    char *token;
    first_line = strsep(&req_body, "\r\n");
    printf("request_str: %s\n\n", first_line);
    // printf("Token: %s\n", first_line);
    for (int i = 0; i < 2; i++)
    {
        token = strsep(&first_line, " ");
        // printf("Token: %s\n", token);
    }
    strcat(path, token);
    if (strcmp(path, "./public/") == 0)
        strcat(path, "index.html");

    return path;
}