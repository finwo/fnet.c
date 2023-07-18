#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "tidwall/buf.h"

#include "fnet.h"

struct fnet_internal_t {
  struct fnet_t ext; // KEEP AT TOP, allows casting between fnet_internal_t* and fnet_t*
};

struct fnet_t * fnet_listen(const char *address, uint16_t port, struct fnet_connect_options_t *options) {
  return NULL;
}

struct fnet_t * fnet_connect(const char *address, uint16_t port, struct fnet_connect_options_t *options) {
  return NULL;
}

void fnet_process(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;
}
void fnet_write(struct fnet_t *connection, struct buf *buf) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;
}

void fnet_close(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;
}

void fnet_free(struct fnet_t *connection) {
  struct fnet_internal_t *conn = (struct fnet_internal_t *)connection;
  free(conn);
}


#ifdef __cplusplus
} // extern "C"
#endif
