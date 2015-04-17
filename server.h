#ifndef SERVER_H
#define SERVER_H

typedef struct server server;

server* server_new(int port);
void server_accept(server* s);
void server_free(server* s);

int server_try_send(server* s, void const* data, unsigned size);
int server_try_recv(server* s, void* data, unsigned size);

#endif
