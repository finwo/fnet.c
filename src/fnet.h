#ifndef __INCLUDE_FINWO_FNET_H__
#define __INCLUDE_FINWO_FNET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "tidwall/buf.h"

#define FNET_SOCKET int

#define FNET_FLAG            uint8_t
#define FNET_FLAG_RECONNECT  1

#define FNET_PROTOCOL  uint8_t
#define FNET_PROTO_TCP 0

#define FNET_STATUS             uint8_t
#define FNET_STATUS_CONNECTING  1   // Client-only status
#define FNET_STATUS_CONNECTED   2   // Client = connected, server = listening
#define FNET_STATUS_ERROR       4
#define FNET_STATUS_CLOSED      8

#define FNET_CALLBACK(NAME) void (*(NAME))(struct fnet_t *connection, void *udata)
#define FNET_CALLBACK_VA(NAME, ...) void (*(NAME))(struct fnet_t *connection, __VA_ARGS__, void *udata)

struct fnet_t {
  FNET_PROTOCOL proto;
  FNET_STATUS   status;
  void *udata;
};

struct fnet_connect_options_t {
  FNET_PROTOCOL proto;
  FNET_FLAG     flags;
  FNET_CALLBACK(onConnect);
  FNET_CALLBACK_VA(onData, struct buf *data);
  FNET_CALLBACK(onClose);
  void *udata;
};

struct fnet_t * fnet_listen(const char *address, uint16_t port, struct fnet_connect_options_t *options);
struct fnet_t * fnet_connect(const char *address, struct fnet_connect_options_t *options);

void fnet_process(struct fnet_t *connection);
void fnet_write(struct fnet_t *connection, struct buf *buf);
void fnet_close(struct fnet_t *connection);
void fnet_free(struct fnet_t *connection);

#ifdef __cplusplus
}
#endif

#endif // __INCLUDE_FINWO_FNET_H__
