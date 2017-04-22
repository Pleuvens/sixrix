# Simple SDL mini code

CC=gcc
CPPFLAGS= `pkg-config --cflags sdl`
CFLAGS= -Wall -Werror -Wextra -std=c99 -pedantic -g3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_ttf

SRC1= main.c
OBJ1= ${SRC1:.c=.o}

all: main

main: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f *~ *.d main

# END
