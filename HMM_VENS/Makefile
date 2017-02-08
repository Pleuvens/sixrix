#
# Makefile contents
#
CC= gcc

CPPFLAGS= -lm
CFLAGS= -Wall -Wextra -Werror -std=c99 -g3
LDFLAGS=
LDLIBS= -lm

SRC= hmm.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: hmm

-include ${DEP}

GUI: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} hmm vgcore.*

# END
