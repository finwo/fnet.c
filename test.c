#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "fnet.h"

int main() {
  const struct fnet_options_t opts = {
    .proto     = FNET_PROTO_TCP,
    .flags     = 0,
    .onConnect = NULL,
    .onData    = NULL,
    .onClose   = NULL,
    .udata     = NULL,
  };

  struct fnet_t *conn = fnet_listen("::", 1337, &opts);

  printf("p: %p\n", conn);

  sleep(3600);


  return 42;
}
