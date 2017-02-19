# Makefile

CC=gcc

CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDLIBS= -lm
SRC1= fft.c
OBJ1= ${SRC1:.c=.o}

all: fft

test: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f fft

