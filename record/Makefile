#Makefile
CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c99 -g -lm
LDFLAGS=
LDLIBS=	 -lm -lrt -lasound  -lpthread -lportaudio -lsndfile

SRC= test.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: test

-include ${DEP}

hanning: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} test

# END
