#!/bin/sh
rm -f Makefile
echo "CC = gcc" >> Makefile 
echo "BIN = philo" >> Makefile 
echo "CFLAGS =  -Wall -pedantic -W -O2 -g" >> Makefile 
echo "LDFLAGS =  -Wall -pedantic -W -O2 -lreadline -ltermcap -g" >> Makefile 

O=""
oo=""

for file in `ls *.c`
do
  oo=`echo $file |sed s/c$/o/g`
  O="$O $oo"
done

echo 'O = '$O >> Makefile 
echo '' >> Makefile 
echo '$(BIN): $(O)' >> Makefile 
echo '	$(CC) $(LDFLAGS) -o $(BIN) $(O)' >> Makefile 
echo '' >> Makefile

for file in `ls *.c`
do
  gcc -MM $file >> Makefile
  oo=`echo $file |sed s/c$/o/g`
  echo '	$(CC) $(CFLAGS) -c '$file  '-o '$oo>> Makefile 
  echo >> Makefile 
done


echo 'clean:' >> Makefile
echo '	rm -f $(O) $(BIN) *~' >> Makefile
