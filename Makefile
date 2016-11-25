src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = main

CC = gcc
CFLAGS = -Wall -g -O2
LDFLAGS =

all : $(target)

$(target) : $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o
	rm -rf main
	rm -rf *~

.PHONY:clean
