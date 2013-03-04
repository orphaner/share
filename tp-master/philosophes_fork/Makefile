CC = gcc
BIN = philo
CFLAGS =  -Wall -pedantic -W -O2 -g
LDFLAGS =  -Wall -pedantic -W -O2 -lreadline -ltermcap -g
O =  philo_nico.o

$(BIN): $(O)
	$(CC) $(LDFLAGS) -o $(BIN) $(O)

philo_nico.o: philo_nico.c
	$(CC) $(CFLAGS) -c philo_nico.c -o philo_nico.o

clean:
	rm -f $(O) $(BIN) *~
