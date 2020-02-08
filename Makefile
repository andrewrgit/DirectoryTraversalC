CC=gcc
CFLAGS= -g
Dependancies = queue.h
objects = main.o queue.o

%.o: %.c $(Dependancies)
	$(CC) -c -o $@ $< $(CFLAGS)

bt: $(objects)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f bt
