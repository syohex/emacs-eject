EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS)

all: eject.so

eject.so: eject.o
	$(LD) -shared $(LDFLAGS) -o $@ $^

eject.o: eject.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f eject.o eject.so
