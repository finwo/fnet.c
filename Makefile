LIBS:=
SRC:=

BIN?=fnet_test
SRC+=test.c

SRC+=$(wildcard src/*.c)
SRC+=$(wildcard src/*/*.c)

override CFLAGS?=-Wall -O2

INCLUDES:=
INCLUDES+=-I src

ifeq ($(OS),Windows_NT)
    # CFLAGS += -D WIN32
    # override CFLAGS+=-I external/libs/Vanara.PInvoke.Ws2_32.3.4.17/build/native/include
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        # CFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            # CFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            # CFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # CFLAGS += -D LINUX
        # override CFLAGS+=$(shell pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0 glib-2.0)
    endif
    ifeq ($(UNAME_S),Darwin)
        # CFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        # CFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        # CFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        # CFLAGS += -D ARM
    endif
endif

include lib/.dep/config.mk

OBJ:=$(SRC:.c=.o)
OBJ:=$(OBJ:.cc=.o)

override CFLAGS+=$(INCLUDES)

LDFLAGS?=-s
LDFLAGS+=$(CFLAGS)

default: $(BIN)

$(OBJ): $(SRC)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
