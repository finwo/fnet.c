fnet
====

## Installation

The easiest way to add fnet to your project is to use [dep][dep] to install it
as a dependency, ensure you include lib/.dep/config.mk in your makefile, and add
`#include "finwo/fnet.h"` in the files where you're using it.

```sh
dep add finwo/fnet
```

## Usage

### Connect to remote

```c
#include <string.h>
#include "finwo/fnet.h"
#include "tidwall/buf.h"

void onData(struct fnet_ev *ev) {
  printf("Data received: %s\n", ev->buffer->data);
}

void onTick(struct fnet_ev *ev) {
  const char *data = "Hello world!";
  int cnt = *((int*)ev->udata);

  fnet_write(ev->connection, &((struct buf){
    .len  = strlen(data) + 1,
    .data = data,
  }));

  cnt++;
  *((int*)ev->udata) = cnt;
  if (cnt > 10) {
    fnet_close(ev->connection);
    exit(0);
  }
}

int main() {
  int cnt = 0;

  fnet_connect(addr, port, &((struct fnet_options_t){
    .proto     = FNET_PROTO_TCP,
    .flags     = 0,
    .onConnect = NULL,
    .onData    = onData,
    .onTick    = onTick,
    .onClose   = NULL,
    .udata     = &cnt,
  }));

  fnet_main();
  return 0;
}
```

### Listen for connections

```c
#include "finwo/fnet.h"

// Just an echo
void onData(struct fnet_ev *ev) {
  fnet_write(ev->connection, ev->buffer);
}

// Attach our onData to the connection
void onConnect(struct fnet_ev *ev) {
  ev->connection->onData  = onData;
}

int main() {
    fnet_listen("0.0.0.0", 80, &((struct fnet_options_t){
        .proto     = FNET_PROTO_TCP,
        .flags     = 0,
        .onConnect = onConnect,
        .onData    = NULL,
        .onTick    = NULL,
        .onClose   = NULL,
        .udata     = NULL,
    }));

    fnet_main();
    return 0;
}
```

[dep]: https://github.com/finwo/dep
