#!/bin/sh

for i in `ipcs | grep 674 | cut -d " " -f2`; 
  do 
  ipcrm -m $i; 
  ipcrm -s $i; 
done;