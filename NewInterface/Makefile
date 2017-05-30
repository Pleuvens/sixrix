# Simple SDL mini code

CC=gcc
CPPFLAGS= -rdynamic -MMD -D_XOPEN_SOURCE=500 `pkg-config --cflags gtk+-3.0 sdl`
CFLAGS= -Wall -Werror -Wextra -std=c99 -pedantic -g3
LDFLAGS=
LDLIBS= -lm `pkg-config --libs gtk+-3.0 sdl` -lSDL_ttf -rdynamic -lrt -lasound -lpthread -lportaudio -lsndfile

SRC1= main.c
OBJ1= ${SRC1:.c=.o}

all: main

main: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f *~ *.d main

# END
