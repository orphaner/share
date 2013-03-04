CC = gcc
BIN = philo_t
CFLAGS = -Wall -pedantic -W -O2 -g
LDFLAGS =  -Wall -pedantic -W -O2 -lpthread -g
O =  output/erreur.o output/memutils.o output/philo_t.o output/sem.o

$(BIN): $(O)
	$(CC) $(LDFLAGS) -o $(BIN) $(O)

output/erreur.o: erreur.c erreur.h
	$(CC) $(CFLAGS) -c erreur.c -o output/erreur.o

output/memutils.o: memutils.c erreur.h
	$(CC) $(CFLAGS) -c memutils.c -o output/memutils.o

output/philo_t.o: philo_t.c memutils.h erreur.h sem.h philo_t.h
	$(CC) $(CFLAGS) -c philo_t.c -o output/philo_t.o

output/sem.o: sem.c erreur.h
	$(CC) $(CFLAGS) -c sem.c -o output/sem.o

clean:
	rm -f $(O) $(BIN) *~
all:
	make clean
	make
