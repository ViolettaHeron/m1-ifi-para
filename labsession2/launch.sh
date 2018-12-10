#!/bin/bash
# note : if you want to pass any arguments to your main, put them after the name of the file you wish to compile and time

echo "Compilation : $1.c"

echo "--- sequencial version"
gcc -o $1.out ./$1.c -lm
echo "--- parallel version"
gcc -o $1-para.out ./$1.c -fopenmp -lm
echo                                          # empty line for output lisibility



 export TIMEFORMAT='%3R'
 echo "timing program for different sample sizes: $1.c"

 # nbThreads=(1 2 3 4 5 6)
 arrayLengths=(100 1000 10000 100000 500000 1000000 10000000)
 arrayPow=(4 8 16 18 20 22 23 24 25 26)

 touch $1.csv
 echo "sample(power of 2),parallel,sequencial" > ./$1.csv

 for i in "${arrayPow[@]}"
 do
   echo "For a sample of size $i"
   # line="$t,"
   line=""
   line="$line$i,"
   line="$line$({ time ./$1-para.out $i 2>&3; } 3>&2 2>&1),"
   line="$line$({ time ./$1.out $i 2>&3; } 3>&2 2>&1)"
   # line="$line$(./$1-para $i),"
   # line="$line$(./$1 $i)"

   echo $line >> $1.csv
   # { time ./$1 $i 2>&3; } 3>&2 2>> $1.csv
 done

 echo "Done! You can find output in $1.csv"


# parallelTime=$(time ./$1-para $i 2>&1)
# echo $parallelTime  >> $1.csv
# sequencialTime=$(time ./$1 $i 2>&1)
# echo $sequencialTime >> $1.csv
