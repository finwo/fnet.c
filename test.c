#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "fnet.h"

int main() {
  uint16_t us = 1337;
  /* uint32_t ui = 1337; */

  const struct fnet_options_t opts = {
    .proto     = FNET_PROTO_TCP,
    .flags     = 0,
    .onConnect = NULL,
    .onData    = NULL,
    .onClose   = NULL,
    .udata     = NULL,
  };

  struct fnet_t *conn = fnet_listen("::", 1337, &opts);

  sleep(3600);

  printf("d: %d\n", us);
  printf("h: %i\n", us);

  return 42;
}
