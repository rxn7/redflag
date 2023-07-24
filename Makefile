CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb
.PHONY: all install

all: example example2

example:
	$(CC) $(CFLAGS) -o example example.c

example2:
	$(CC) $(CFLAGS) -o example2 example2.c

install:
	sudo cp reflag.h /usr/local/include/redflag.h

clean:
	rm example example2
