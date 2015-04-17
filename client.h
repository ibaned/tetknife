#ifndef CLIENT_H
#define CLIENT_H

typedef struct client client;

client* client_new(char const* servname, int port);
void client_free(client* c);

int client_try_send(client* c, void const* data, unsigned size);
int client_try_recv(client* c, void* data, unsigned size);

#endif
