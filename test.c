#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "fnet.h"

void onClose(struct fnet_ev *ev) {
  printf("Connection closed!\n");
}

void onData(struct fnet_ev *ev) {
  printf("Data(%d): %.*s\n", ev->buffer->len, (int)(ev->buffer->len), ev->buffer->data);

  // Simple echo
  fnet_write(ev->connection, ev->buffer);
}

void onConnect(struct fnet_ev *ev) {
  printf("Connection!!\n");
  ev->connection->onData  = onData;
  ev->connection->onClose = onClose;
}

int main() {
  const struct fnet_options_t opts = {
    .proto     = FNET_PROTO_TCP,
    .flags     = 0,
    .onConnect = onConnect,
    .onData    = NULL,
    .onClose   = NULL,
    .udata     = NULL,
  };

  fnet_listen("0.0.0.0", 1337, &opts);
  fnet_main();

  return 42;
}
