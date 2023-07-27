#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include "tidwall/buf.h"

#include "fnet.h"

#define FNET_SOCKET int

struct fnet_internal_t {
  struct fnet_t ext; // KEEP AT TOP, allows casting between fnet_internal_t* and fnet_t*
  FNET_SOCKET   *fds;
  int           nfds;
  FNET_FLAG     flags;
  FNET_CALLBACK(onConnect);
  FNET_CALLBACK_VA(onData, struct buf *data);
  FNET_CALLBACK(onClose);
};

int settcpnodelay(int fd) {
  return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &(int){1}, sizeof(int));
}

int setnonblock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) return flags;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

struct fnet_t * fnet_listen(const char *address, uint16_t port, const struct fnet_options_t *options) {
  struct fnet_internal_t *conn;

  // Checking arguments are given
  if (!address) {
    fprintf(stderr, "fnet_listen: address argument is required\n");
    return NULL;
  }
  if (!port) {
    fprintf(stderr, "fnet_listen: port argument is required\n");
    return NULL;
  }
  if (!options) {
    fprintf(stderr, "fnet_listen: options argument is required\n");
    return NULL;
  }

  // Check if we support the protocol
  switch(options->proto) {
    case FNET_PROTO_TCP:
      // Intentionally empty
      // TODO: tcp-specific arg validation
      break;
    default:
      fprintf(stderr, "fnet_listen: unknown protocol\n");
      return NULL;
  }

  // 1-to-1 copy, don't touch the options
  conn = malloc(sizeof(struct fnet_internal_t));
  conn->ext.proto  = options->proto;
  conn->ext.status = FNET_STATUS_INITIALIZING;
  conn->ext.udata  = options->udata;
  conn->flags      = options->flags;
  conn->onConnect  = options->onConnect;
  conn->onData     = options->onData;
  conn->onClose    = options->onClose;
  conn->nfds       = 0;
  conn->fds        = NULL;

  /* struct sockaddr_in servaddr; */
  struct addrinfo hints = {}, *addrs;
  char port_str[6] = {};

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Get address info
  snprintf(port_str, sizeof(port_str), "%d", port);
  int ai_err = getaddrinfo(address, port_str, &hints, &addrs);
  if (ai_err != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ai_err));
    fnet_free((struct fnet_t *)conn);
    return NULL;
  }

  // Count the addresses to listen on
  // For example, "localhost" turned to "127.0.0.1" and "::1"
  int naddrs = 0;
  struct addrinfo *addrinfo = addrs;
  while(addrinfo) {
    naddrs++;
    addrinfo = addrinfo->ai_next;
  }

  conn->fds = calloc(naddrs, sizeof(FNET_SOCKET));
  if (!conn->fds) {
    fprintf(stderr, "%s\n", strerror(ENOMEM));
    fnet_free((struct fnet_t *)conn);
    return NULL;
  }

  addrinfo = addrs;
  for (; addrinfo ; addrinfo = addrinfo->ai_next ) {

    int fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
    if (fd < 0) {
      fprintf(stderr, "socket\n");
      fnet_free((struct fnet_t *)conn);
      return NULL;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
      fprintf(stderr, "setsockopt(SO_REUSEADDR)\n");
      fnet_free((struct fnet_t *)conn);
      return NULL;
    }

    if (setnonblock(fd) < 0) {
      fprintf(stderr, "setnonblock\n");
      fnet_free((struct fnet_t *)conn);
      return NULL;
    }

    if (bind(fd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
      fprintf(stderr, "bind\n");
      fnet_free((struct fnet_t *)conn);
      return NULL;
    }

    if (listen(fd, SOMAXCONN) < 0) {
      fprintf(stderr, "bind\n");
      fnet_free((struct fnet_t *)conn);
      return NULL;
    }

    conn->fds[conn->nfds] = fd;
    conn->nfds++;
  }

  freeaddrinfo(addrs);

  return conn;
}

struct fnet_t * fnet_connect(const char *address, uint16_t port, const struct fnet_options_t *options) {

  // Checking arguments are given
  if (!address) {
    fprintf(stderr, "fnet_connect: address argument is required\n");
    return NULL;
  }
  if (!port) {
    fprintf(stderr, "fnet_listen: port argument is required\n");
    return NULL;
  }
  if (!options) {
    fprintf(stderr, "fnet_connect: options argument is required\n");
    return NULL;
  }

  // Check if we support the protocol
  switch(options->proto) {
    case FNET_PROTO_TCP:
      // Intentionally empty
      break;
    default:
      fprintf(stderr, "fnet_connect: unknown protocol\n");
      return NULL;
  }


  return NULL;
}

FNET_RETURNCODE fnet_process(const struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_process: connection argument is required\n");
    return FNET_RETURNCODE_MISSING_ARGUMENT;
  }

  return FNET_RETURNCODE_OK;
}



FNET_RETURNCODE fnet_write(const struct fnet_t *connection, struct buf *buf) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_write: connection argument is required\n");
    return FNET_RETURNCODE_MISSING_ARGUMENT;
  }
  if (!buf) {
    fprintf(stderr, "fnet_write: buf argument is required\n");
    return FNET_RETURNCODE_MISSING_ARGUMENT;
  }

  return FNET_RETURNCODE_OK;
}

FNET_RETURNCODE fnet_close(const struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_close: connection argument is required\n");
    return FNET_RETURNCODE_MISSING_ARGUMENT;
  }

  return FNET_RETURNCODE_OK;
}

FNET_RETURNCODE fnet_free(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_free: connection argument is required\n");
    return FNET_RETURNCODE_MISSING_ARGUMENT;
  }

  // TODO: check if the connections are closed?

  if (conn->fds) free(conn->fds);

  free(conn);

  return FNET_RETURNCODE_OK;
}

FNET_RETURNCODE fnet_step() {
  // TODO: process all open fnet instances
  return FNET_RETURNCODE_OK;
}

FNET_RETURNCODE fnet_main() {
  FNET_RETURNCODE ret;
  while(1) {
    // TODO: handle kill signal?
    // TODO: dynamic sleep to have 10ms - 100ms tick?
    ret = fnet_step();
    if (ret) return ret;
  }

  // TODO: is this really ok?
  return FNET_RETURNCODE_OK;
}



#ifdef __cplusplus
} // extern "C"
#endif
