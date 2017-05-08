#Makefile
CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra  -std=c99 -g -lm
LDFLAGS=
LDLIBS=	 -lm -lrt -lasound  -lpthread -lportaudio -lsndfile

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

-include ${DEP}

hanning: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} main

remove:
		rm -f ${OBJ} ${DEP} main *.raw *.wav *.flac

# END
