#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "tidwall/buf.h"

#include "fnet.h"

struct fnet_internal_t {
  struct fnet_t ext; // KEEP AT TOP, allows casting between fnet_internal_t* and fnet_t*
  FNET_SOCKET   sock;
  FNET_FLAG     flags;
  FNET_CALLBACK(onConnect);
  FNET_CALLBACK_VA(onData, struct buf *data);
  FNET_CALLBACK(onClose);
};

struct fnet_t * fnet_listen(const char *address, uint16_t port, struct fnet_connect_options_t *options) {

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
      // TODO: something
      break;
    default:
      fprintf(stderr, "fnet_listen: unknown protocol\n");
      return NULL;
  }

  return NULL;
}

struct fnet_t * fnet_connect(const char *address, struct fnet_connect_options_t *options) {

  // Checking arguments are given
  if (!address) {
    fprintf(stderr, "fnet_connect: address argument is required\n");
    return NULL;
  }
  if (!options) {
    fprintf(stderr, "fnet_connect: options argument is required\n");
    return NULL;
  }

  return NULL;
}

void fnet_process(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_process: connection argument is required\n");
    return NULL;
  }

}
void fnet_write(struct fnet_t *connection, struct buf *buf) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_write: connection argument is required\n");
    return NULL;
  }
  if (!buf) {
    fprintf(stderr, "fnet_write: buf argument is required\n");
    return NULL;
  }

}

void fnet_close(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_close: connection argument is required\n");
    return NULL;
  }
}

void fnet_free(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;

  // Checking arguments are given
  if (!conn) {
    fprintf(stderr, "fnet_free: connection argument is required\n");
    return NULL;
  }

  free(conn);
}


#ifdef __cplusplus
} // extern "C"
#endif
