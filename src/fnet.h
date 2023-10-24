#ifndef __INCLUDE_FINWO_FNET_H__
#define __INCLUDE_FINWO_FNET_H__

#include <stdint.h>

#include "tidwall/buf.h"

#define FNET_FLAG            uint8_t
#define FNET_FLAG_RECONNECT  1

#define FNET_PROTOCOL  uint8_t
#define FNET_PROTO_TCP 0

#define FNET_RETURNCODE                  int
#define FNET_RETURNCODE_OK               0
#define FNET_RETURNCODE_ERROR            -1
#define FNET_RETURNCODE_MISSING_ARGUMENT -2
#define FNET_RETURNCODE_NOT_IMPLEMENTED  -3
#define FNET_RETURNCODE_ERRNO            -4
#define FNET_RETURNCODE_UNPROCESSABLE    -5

#define FNET_STATUS               uint8_t
#define FNET_STATUS_INITIALIZING   0
#define FNET_STATUS_CONNECTING     1 // Client-only status
#define FNET_STATUS_CONNECTED      2 // Connection ready
#define FNET_STATUS_LISTENING      4 // Listen ready
#define FNET_STATUS_READY          6 // Any ready
#define FNET_STATUS_ACCEPTED       8 // Whether the connection is an accepted one
#define FNET_STATUS_ERROR         16
#define FNET_STATUS_CLOSED        32

#define FNET_EVENT         int
#define FNET_EVENT_LISTEN  1
#define FNET_EVENT_CONNECT 2
#define FNET_EVENT_DATA    3
#define FNET_EVENT_TICK    4
#define FNET_EVENT_CLOSE   5

#define FNET_CALLBACK(NAME) void (*(NAME))(struct fnet_ev *event)

struct fnet_ev {
  struct fnet_t *connection;
  FNET_EVENT     type;
  struct buf    *buffer;
  void          *udata;
};

struct fnet_t {
  FNET_PROTOCOL proto;
  FNET_STATUS   status;
  FNET_CALLBACK(onListen);
  FNET_CALLBACK(onConnect);
  FNET_CALLBACK(onData);
  FNET_CALLBACK(onTick);
  FNET_CALLBACK(onClose);
  void *udata;
};

struct fnet_options_t {
  FNET_PROTOCOL proto;
  FNET_FLAG     flags;
  FNET_CALLBACK(onListen);
  FNET_CALLBACK(onConnect);
  FNET_CALLBACK(onData);
  FNET_CALLBACK(onTick);
  FNET_CALLBACK(onClose);
  void *udata;
};

struct fnet_t * fnet_listen(const char *address, uint16_t port, const struct fnet_options_t *options);
struct fnet_t * fnet_connect(const char *address, uint16_t port, const struct fnet_options_t *options);

FNET_RETURNCODE fnet_process(const struct fnet_t *connection);
FNET_RETURNCODE fnet_write(const struct fnet_t *connection, struct buf *buf);
FNET_RETURNCODE fnet_close(const struct fnet_t *connection);
FNET_RETURNCODE fnet_free(struct fnet_t *connection);

FNET_RETURNCODE fnet_tick();
FNET_RETURNCODE fnet_main();

#endif // __INCLUDE_FINWO_FNET_H__
