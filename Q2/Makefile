SHELL = /bin/sh

OBJS = text2epub.c
CFLAGS = -Wall -Werror
CC = gcc
INCLUDE =
LIBS =

samples: text2epub.c
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean: 
	-rm -f *.o core *.core text2epub

.o: ${CC} ${CFLAGS} ${INCLUDES} -c $<
