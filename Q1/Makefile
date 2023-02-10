SHELL = /bin/sh

OBJS = samples.c
CFLAGS = -Wall -Werror
CC = gcc
INCLUDE =
LIBS =

samples: samples.c
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean: 
	-rm -f *.o core *.core samples

.o: ${CC} ${CFLAGS} ${INCLUDES} -c $<