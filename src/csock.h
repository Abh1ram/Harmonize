#ifndef CSOCK_H
#define CSOCK_H

#include <cstdio>

enum csockerror {
	EGETADDRINFO = 0x01,
	ESETSOCKOPT,
	ENOBIND,
	ELISTEN,
	ESIGACTION,
	ENOACCEPT,
	ENOSEND
};

struct csock_listener {
	int sockfd;
	bool is_passive;
};

struct csock_connection {
	int sockfd;
	struct sockaddr_storage their_addr;
	bool is_open;
	struct FILE *sockfile;
}

// Listens and accepts new connections
int listen_and_accept(struct csock_listener *listener,
                      struct csock_connection *conn,
                      const char *port = 0);
int csock_send(const struct csock_connection &conn,
               char *buf, int size);
int csock_close(const struct csock_connection &conn);

#endif