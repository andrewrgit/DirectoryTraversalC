CC=gcc
CFLAGS= -g -Wall
Dependancies = queue.h
objects = main.o queue.o

%.o %.c $(Dependancies)
	$(CC) -c -o $@ $< $(CFLAGS)

main.o : queue.h
queue.o : queue.h

clean:
	rm *.o
