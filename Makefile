all: opengh

opengh: slre.o opengh.o
	gcc -o opengh opengh.o slre.o
	
slre.o: lib/slre.c
	gcc -o slre.o -c lib/slre.c -W -Wall -ansi -pedantic
	
opengh.o: opengh.c lib/slre.h
	gcc -o opengh.o -c opengh.c -W -Wall -ansi -pedantic
	
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf opengh