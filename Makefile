# See LICENSE file for copyright and license details.
# SPDX-License-Identifier: MIT

PREFIX ?= /usr

CFLAGS ?= -O2 -pipe
CFLAGS += -Wall -pedantic
CFLAGS += -Werror=implicit-function-declaration

VERSION = 0.0

BIN = cii ut2lt

all: $(BIN)

clean:
	$(RM) $(BIN) *.o

install: all
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin

.PHONY: all clean install
