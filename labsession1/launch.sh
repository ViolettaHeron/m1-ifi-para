#!/bin/bash
# note : if you want to pass any arguments to your main, put them after the name of the file you wish to compile and time

echo "Compilation : $1.c"

echo "--- sequencial version"
gcc -o $1 ./$1.c -lm
echo "--- parallel version"
gcc -o $1-para ./$1.c -fopenmp -lm
echo                                          # empty line for output lisibility


export TIMEFORMAT='%3R'
echo "timing program for different sample sizes: $1.c"

nbThreads=(1 2 3 4 5 6)
arrayLengths=(100 1000 10000 100000 1000000 10000000 50000000 100000000)

touch $1.csv
echo "threads,sample,parallel" > ./$1.csv


for t in "${nbThreads[@]}"
do
  for i in "${arrayLengths[@]}"
  do
    echo "$t threads for a sample of size $i"
    line="$t,"
    line="$line$i,"
    line="$line$({ time ./$1-para $i $t 2>&3; } 3>&2 2>&1)"
    echo $line >> $1.csv
    # { time ./$1 $i 2>&3; } 3>&2 2>> $1.csv
  done
done

echo "Done! You can find output in $1.csv"


# parallelTime=$(time ./$1-para $i 2>&1)
# echo $parallelTime  >> $1.csv
# sequencialTime=$(time ./$1 $i 2>&1)
# echo $sequencialTime >> $1.csv
