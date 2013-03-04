#!/bin/sh
rm -f Makefile

odir="output";

if  [ ! -d $odir ] 
    then
    mkdir $odir;
fi;

echo "CC = gcc" >> Makefile 
echo "BIN = client" >> Makefile 
echo "CFLAGS = -Wall -pedantic -W -O2 -g" >> Makefile 
echo "LDFLAGS =  -Wall -pedantic -W -O2 -lpthread -g" >> Makefile 

O=""
oo=""

for file in `ls *.c`
do
  oo=`echo $file |sed s/c$/o/g`
  O="$O $odir/$oo"
done

echo 'O = '$O >> Makefile 
echo '' >> Makefile 
echo '$(BIN): $(O)' >> Makefile 
echo '	$(CC) $(LDFLAGS) -o $(BIN) $(O)' >> Makefile 
echo '' >> Makefile

for file in `ls *.c`
do
  echo -n "$odir/" >> Makefile
  gcc -MM $file >> Makefile
  oo=`echo $odir/$file |sed s/c$/o/g`
  echo '	$(CC) $(CFLAGS) -c '$file  '-o '$oo>> Makefile 
  echo >> Makefile 
done

echo 'clean:' >> Makefile
echo '	rm -f $(O) $(BIN) *~' >> Makefile

echo 'all:' >> Makefile
echo '	make clean' >> Makefile
echo '	make' >> Makefile
