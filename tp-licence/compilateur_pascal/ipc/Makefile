CC = gcc
BIN = ipc
CFLAGS =  -Wall -Werror -ansi -pedantic -W -O2 -g
O =  erreur.o interpreteur.o lecture_fichier.o lire_inst.o main.o pile_exe.o pile_var.o ../common/mnemonique.o

$(BIN): $(O)
	$(CC) $(CFLAGS) -o $(BIN) $(O)

erreur.o: erreur.c lire_inst.h lecture_fichier.h ../common/mnemonique.h \
  pile_exe.h pile_var.h main.h erreur.h
	$(CC) $(CFLAGS) -c erreur.c -o erreur.o

interpreteur.o: interpreteur.c ../common/mnemonique.h erreur.h pile_exe.h \
  lire_inst.h lecture_fichier.h pile_var.h interpreteur.h
	$(CC) $(CFLAGS) -c interpreteur.c -o interpreteur.o

lecture_fichier.o: lecture_fichier.c erreur.h lecture_fichier.h
	$(CC) $(CFLAGS) -c lecture_fichier.c -o lecture_fichier.o

lire_inst.o: lire_inst.c lire_inst.h lecture_fichier.h \
  ../common/mnemonique.h erreur.h pile_exe.h
	$(CC) $(CFLAGS) -c lire_inst.c -o lire_inst.o

main.o: main.c ../common/mnemonique.h erreur.h lecture_fichier.h \
  lire_inst.h pile_exe.h pile_var.h interpreteur.h main.h
	$(CC) $(CFLAGS) -c main.c -o main.o

pile_exe.o: pile_exe.c ../common/mnemonique.h erreur.h lire_inst.h \
  lecture_fichier.h pile_exe.h
	$(CC) $(CFLAGS) -c pile_exe.c -o pile_exe.o

pile_var.o: pile_var.c erreur.h lire_inst.h lecture_fichier.h \
  ../common/mnemonique.h pile_var.h
	$(CC) $(CFLAGS) -c pile_var.c -o pile_var.o

"../common/mnemonique.o": ../common/mnemonique.c ../common/mnemonique.h
	$(CC) $(CFLAGS) -c ../common/mnemonique.c -o ../common/mnemonique.o
clean:
	rm -f $(O) $(BIN) *~ 
backup:
	rm -f sav/pp1.tar.bz2
	tar cjf sav/pp1.tar.bz2 Makefile *.c *.h README configure.sh
