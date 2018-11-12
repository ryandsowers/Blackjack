# ------------------------------------------------------------------------
#  This is the Make file for the blackjack program
#
#  Created: 2016-05-03 (P. Clark)
#
#  Modifications:
#  2017-04-18 (P. Clark)
#      Added a test target as a way of testing card.c.
#  2017-10-30 (P. Clark)
#      Updated dependencies and options.
# ------------------------------------------------------------------------


OBJECTS=main.o table.o card.o

CFLAGS=-g -Wall -c
LDFLAGS=-o

all: blackjack


blackjack: $(OBJECTS)
	gcc $(OBJECTS) $(LDFLAGS) blackjack

test: test.o card.o
	gcc test.o card.o -o test

main.o: main.c table.h common.h card.h
	gcc $(CFLAGS) main.c

table.o: table.c table.h common.h card.h
	gcc $(CFLAGS) table.c

card.o: card.c card.h common.h
	gcc $(CFLAGS) card.c

test.o: test.c card.h common.h
	gcc $(CFLAGS) test.c

clean:
	rm -f $(OBJECTS) blackjack test test.o

