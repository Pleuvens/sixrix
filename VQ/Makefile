#
# Makefile contents
#
CC= gcc

CPPFLAGS= -lm -rdynamic -MMD `pkg-config --cflags gtk+-3.0 sdl`
CFLAGS= -Wall -Werror -Wextra -std=c99 -pedantic -g3
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0 sdl` -lSDL_image -lm -rdynamic

SRC= vq.c ../mfcc/mfcc.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: vq

-include ${DEP}

GUI: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} vq vgcore.*

# END
