/*
 * server.c -- a very simple stream socket server implementation.
 * This implementation is quick and dirty and largely a modded form
 * of the one found on http://beej.us/guide/bgnet/html/single/bgnet.html
 * Unless absolutely necessary, don't poke around in this code.
 */

// TODO Read the docs and replace this entire thing with 
// boost.asio implementation
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <csock.h>

#define BACKLOG 10     // how many pending connections queue will hold

using namespace std;

void sigchld_handler(int s) {
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



// sock_fd represents the socket to listen on
int listen_and_accept(struct csock_listener *listener,
                      struct csock_connection *conn,
                      const char *port = 0) {
  if (!listener)
    return -1;

  int new_fd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  if (!listener->is_passive) {
    // Socket is invalid or not set to listening state yet
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
      return EGETADDRINFO;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((listener->sockfd = socket(p->ai_family, p->ai_socktype,
          p->ai_protocol)) == -1) {
        continue;
      }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
          sizeof(int)) == -1) {
        return ESETSOCKOPT;
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        continue;
      }

      break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
      return ENOBIND;
    }

    if (listen(sockfd, BACKLOG) == -1) {
      return ELISTEN;
    }
    else {
      listener->is_passive = true;
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      return ESIGACTION;
    }
  }

  if (conn) {
    // Now wait for connections, and return as soon
    // as one is available
    while(1) {  // main accept() loop
      sin_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (new_fd == -1) {
        return ENOACCEPT;
        continue;
      }
      else {
        conn->their_addr = their_addr;
        conn->sockfd = new_fd;
        conn->sockfile = fdopen(conn->sockfd, "rw");
        conn->is_open = true;
        return 0;
      }

      inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof s);
      printf("server: got connection from %s\n", s);
    }
  }

  return 0;
}

int csock_send(const struct csock_connection &conn,
               char *buf, int size) {
  if (fprintf(conn.sockfile, "%s\n", buf) < 0)
    return ENOSEND;
}

int csock_readline(const struct csock_connection &conn,
                   char *buf, int size) {
  char *ret = fgets(buf, size, conn.sockfile);
  if (!ret) {
    return -1;
  }
  return std::strlen(buf);
}

int csock_close(struct csock_connection &conn) {
  if (conn.sockfd > 0 && conn.is_open) {
    fclose(conn.sockfile);
    conn.sockfd = -1;
    conn.is_open = false;
  }
}
