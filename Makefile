prefix=/usr/local
bindir=$(prefix)/bin
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
EXEC=opengh

all: $(EXEC)

opengh: slre.o opengh.o
	$(CC) -o $@ $^
	
slre.o: lib/slre.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
opengh.o: opengh.c lib/slre.h
	$(CC) -o $@ -c $< $(CFLAGS)

install: all
	install -m 0755 opengh $(bindir)

.PHONY: clean mrproper install

clean:
	rm -rf *.o *~
	
mrproper: clean
	rm -rf $(EXEC)