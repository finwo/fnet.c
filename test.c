#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "fnet.h"

void onConnect(struct fnet_ev *ev) {
  printf("Connection!!\n");
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

  struct fnet_t *conn = fnet_listen("0.0.0.0", 1337, &opts);
  fnet_main();

  return 42;
}
