# Simple SDL mini code

CC=gcc
CPPFLAGS= -lm -rdynamic -MMD `pkg-config --cflags gtk+-3.0 sdl`
CFLAGS= -Wall -Werror -Wextra -std=c99 -pedantic -g3
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0 sdl` -lSDL_image -lm -rdynamic

SRC1= main.c
OBJ1= ${SRC1:.c=.o}

all: main

main: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f *~ *.d main

# END
