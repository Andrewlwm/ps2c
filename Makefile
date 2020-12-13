CC=gcc

all: server client

server: server.c utils.c
	$(CC) -o server server.c utils.c

client: client.c
	$(CC) -o client client.c

clean: 
	rm -f server client