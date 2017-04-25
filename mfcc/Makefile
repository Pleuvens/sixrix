# Simple SDL mini code

CC=gcc
CPPFLAGS= -lm -rdynamic -MMD `pkg-config --cflags gtk+-3.0 sdl`
CFLAGS= -Wall -Werror -Wextra -std=c99 -pedantic -g
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0 sdl` -lSDL_image -lm -rdynamic

SRC1= ../VQ/vq.c mfcc.c
OBJ1= ${SRC1:.c=.o}

all: mfcc

main: ${OBJ1}

clean:
	rm -f *~ *.o
	rm -f *~ *.d mfcc

# END
