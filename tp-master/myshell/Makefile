CC = gcc
BIN = myshell
CFLAGS = -DDEBUG -Wall -pedantic -W -O2 -g
LDFLAGS =  -Wall -pedantic -W -O2 -lreadline -ltermcap -g
O =  output/dyn_string.o output/erreur.o output/hash.o output/list_fg.o output/memutils.o output/myls.o output/myps.o output/myshell.o output/output_ps.o output/saisie.o output/varglob.o output/varloc.o

$(BIN): $(O)
	$(CC) $(LDFLAGS) -o $(BIN) $(O)

output/dyn_string.o: dyn_string.c common.h dyn_string.h memutils.h
	$(CC) $(CFLAGS) -c dyn_string.c -o output/dyn_string.o

output/erreur.o: erreur.c erreur.h
	$(CC) $(CFLAGS) -c erreur.c -o output/erreur.o

output/hash.o: hash.c hash.h memutils.h
	$(CC) $(CFLAGS) -c hash.c -o output/hash.o

output/list_fg.o: list_fg.c common.h dyn_string.h saisie.h list_fg.h memutils.h
	$(CC) $(CFLAGS) -c list_fg.c -o output/list_fg.o

output/memutils.o: memutils.c erreur.h
	$(CC) $(CFLAGS) -c memutils.c -o output/memutils.o

output/myls.o: myls.c myls.h memutils.h
	$(CC) $(CFLAGS) -c myls.c -o output/myls.o

output/myps.o: myps.c myps.h memutils.h output_ps.h
	$(CC) $(CFLAGS) -c myps.c -o output/myps.o

output/myshell.o: myshell.c common.h erreur.h memutils.h dyn_string.h saisie.h \
  myls.h myps.h list_fg.h hash.h varloc.h varglob.h
	$(CC) $(CFLAGS) -c myshell.c -o output/myshell.o

output/output_ps.o: output_ps.c myps.h output_ps.h
	$(CC) $(CFLAGS) -c output_ps.c -o output/output_ps.o

output/	$(CC) $(CFLAGS) -c saisie.c -o output/saisie.o

output/varglob.o: varglob.c hash.h varloc.h varglob.h erreur.h common.h \
  dyn_string.h memutils.h
	$(CC) $(CFLAGS) -c varglob.c -o output/varglob.o

output/varloc.o: varloc.c hash.h varloc.h
	$(CC) $(CFLAGS) -c varloc.c -o output/varloc.o

clean:
	rm -f $(O) $(BIN) *~
all:
	make clean
	make
sem:
	./clean.sh
