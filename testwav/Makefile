# Makefile

CC=gcc

CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDLIBS= -lm
SRC1= wave.c
OBJ1= ${SRC1:.c=.o}

all: wave

test: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f wave

