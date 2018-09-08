CC=gcc
CFLAGS=-std=c99 -Wall -pedantic
DEPS = media_type.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: main.o media_type.o 
	$(CC) -o match main.o media_type.o 
