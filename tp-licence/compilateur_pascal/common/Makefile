CC = gcc
CFLAGS =  -Wall -Werror -ansi -pedantic -W -O2 -g
O =  mnemonique.o

mnemonique.o: mnemonique.c mnemonique.h
	$(CC) $(CFLAGS) -c mnemonique.c

clean:
	rm -f $(O) *~ 
