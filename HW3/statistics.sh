#!/bin/sh
$echo $1
echo making programs
make ram1 ram3_128 ram3_128_32 ram3_128_4 ram3_256
echo creating statistics
./ram1 $1 $2 > ram1_Stats.txt
./ram3_128 $1 $2 > ram3_128_Stats.txt
./ram3_128_4 $1 $2 > ram3_128_4_Stats.txt
./ram3_128_32 $1 $2 > ram3_128_32_Stats.txt
./ram3_256 $1 $2 > ram3_256_Stats.txt
echo clean
make clean