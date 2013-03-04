CC = gcc
BIN = pp5
CFLAGS =  -Wall -Werror -ansi -pedantic -W -O2 -g
O =  erreur.o generation.o hash.o lecture_fichier.o lexical.o librairie.o main.o syntaxique.o typepredef.o ../common/mnemonique.o

$(BIN): $(O)
	$(CC) $(CFLAGS) -o $(BIN) $(O)

erreur.o: erreur.c erreur.h lexical.h lecture_fichier.h syntaxique.h \
  hash.h generation.h ../common/mnemonique.h
	$(CC) $(CFLAGS) -c erreur.c -o erreur.o

generation.o: generation.c ../common/mnemonique.h lecture_fichier.h \
  erreur.h librairie.h generation.h
	$(CC) $(CFLAGS) -c generation.c -o generation.o

hash.o: hash.c syntaxique.h lexical.h lecture_fichier.h hash.h erreur.h \
  typepredef.h
	$(CC) $(CFLAGS) -c hash.c -o hash.o

lecture_fichier.o: lecture_fichier.c lexical.h lecture_fichier.h erreur.h \
  librairie.h
	$(CC) $(CFLAGS) -c lecture_fichier.c -o lecture_fichier.o

lexical.o: lexical.c lecture_fichier.h librairie.h lexical.h
	$(CC) $(CFLAGS) -c lexical.c -o lexical.o

librairie.o: librairie.c librairie.h
	$(CC) $(CFLAGS) -c librairie.c -o librairie.o

main.o: main.c lecture_fichier.h syntaxique.h lexical.h hash.h erreur.h \
  generation.h ../common/mnemonique.h librairie.h
	$(CC) $(CFLAGS) -c main.c -o main.o

syntaxique.o: syntaxique.c hash.h syntaxique.h lexical.h \
  lecture_fichier.h erreur.h generation.h ../common/mnemonique.h \
  librairie.h typepredef.h
	$(CC) $(CFLAGS) -c syntaxique.c -o syntaxique.o

typepredef.o: typepredef.c hash.h lexical.h lecture_fichier.h \
  syntaxique.h erreur.h typepredef.h
	$(CC) $(CFLAGS) -c typepredef.c -o typepredef.o

"../common/mnemonique.o": ../common/mnemonique.c ../common/mnemonique.h
	$(CC) $(CFLAGS) -c ../common/mnemonique.c -o ../common/mnemonique.o
clean:
	rm -f $(O) $(BIN) *~ Gryl_Lassalle.pcode
backup:
	rm -f sav/pp1.tar.bz2
	tar cjf sav/pp1.tar.bz2 Makefile *.c *.h README configure.sh
