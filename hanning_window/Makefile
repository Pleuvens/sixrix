#Makefile
CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -g
LDFLAGS=
LDLIBS=


SRC= hanning.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: hanning

-include ${DEP}

hanning: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} hanning

# END
