CC=gcc
CFLAGS=-Wall -g -o

all: server client

server: server.c utils.c
	$(CC) $(CFLAGS) server server.c utils.c

client: client.c
	$(CC) $(CFLAGS) client client.c

clean: 
	rm -f server client