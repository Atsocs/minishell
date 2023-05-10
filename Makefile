CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

OBJS = caiosh.o

all: caiosh

caiosh: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o caiosh

caiosh.o: caiosh.c caiosh.h
	$(CC) $(CFLAGS) -c caiosh.c

clean:
	rm -f *.o caiosh
