SHELL = /bin/sh

OBJS = tokenring.c
CFLAGS = -Wall -Werror
CC = gcc
INCLUDE =
LIBS =

tokenring: tokenring.c
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean: 
	-rm -f *.o core *.core tokenring

.o: ${CC} ${CFLAGS} ${INCLUDES} -c $<