CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb
.PHONY: all install

all: example

example:
	$(CC) $(CFLAGS) -o example example.c

install:
	sudo cp reflag.h /usr/local/include/redflag.h

clean:
	rm example