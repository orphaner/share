CC = gcc
BIN = serv
CFLAGS = -Wall -ansi -pedantic -W -O2 -g
LDFLAGS =  -Wall -ansi -pedantic -W -O2 -lpthread -g
O =  output/erreur.o output/memutils.o output/net.o output/sem.o output/serveur.o

$(BIN): $(O)
	$(CC) $(LDFLAGS) -o $(BIN) $(O)

output/erreur.o: erreur.c erreur.h
	$(CC) $(CFLAGS) -c erreur.c -o output/erreur.o

output/memutils.o: memutils.c erreur.h
	$(CC) $(CFLAGS) -c memutils.c -o output/memutils.o

output/net.o: net.c erreur.h
	$(CC) $(CFLAGS) -c net.c -o output/net.o

output/sem.o: sem.c erreur.h
	$(CC) $(CFLAGS) -c sem.c -o output/sem.o

output/serveur.o: serveur.c erreur.h memutils.h net.h sem.h
	$(CC) $(CFLAGS) -c serveur.c -o output/serveur.o

clean:
	rm -f $(O) $(BIN) *~
all:
	make clean
	make
