#!/bin/sh
rm -f Makefile
echo "CC = gcc" >> Makefile 
echo "BIN = ipc" >> Makefile 
echo "CFLAGS =  -Wall -Werror -ansi -pedantic -W -O2 -g" >> Makefile 

O=""
oo=""

for file in `ls *.c`
do
  oo=`echo $file |sed s/c$/o/g`
  O="$O $oo"
done
O="$O  ../common/mnemonique.o"

echo 'O = '$O >> Makefile 
echo '' >> Makefile 
echo '$(BIN): $(O)' >> Makefile 
echo '	$(CC) $(CFLAGS) -o $(BIN) $(O)' >> Makefile 
echo '' >> Makefile

for file in `ls *.c`
do
  gcc -MM $file >> Makefile
  oo=`echo $file |sed s/c$/o/g`
  echo '	$(CC) $(CFLAGS) -c '$file  '-o '$oo>> Makefile 
  echo >> Makefile 
done

echo '"../common/mnemonique.o": ../common/mnemonique.c ../common/mnemonique.h' >> Makefile
echo '	$(CC) $(CFLAGS) -c ../common/mnemonique.c -o ../common/mnemonique.o' >> Makefile

echo 'clean:' >> Makefile 
echo '	rm -f $(O) $(BIN) *~ ' >> Makefile 

echo 'backup:' >> Makefile 
echo '	rm -f sav/pp1.tar.bz2' >> Makefile 
echo '	tar cjf sav/pp1.tar.bz2 Makefile *.c *.h README configure.sh' >> Makefile 
make clean
