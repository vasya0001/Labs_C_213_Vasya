#!/bin/bash

gcc -std=c99 -Wall -Wextra task3sem3.c -lm -o main || exit 1

./main
./main -q
./main abc -m
./main -z 1 2
./main -q 0.001 1 2
./main -m 10
./main -t 0.001 3 4

./main -q 0.001 1 -5 6
./main -q 0.001 1 -2 1
./main -q 0.001 1 0 1
./main -q 0.001 0 2 -4
./main -q 0.001 0 0 0
./main -q -0.01 1 2 3
./main -q 1.5 1 2 3
./main -q 0.001 abc 2 3
./main -q 0.001 1e300 1e300 1e300

./main -m 10 5
./main -m 10 3
./main -m -15 5
./main -m 10 0
./main -m abc 5
./main -m 99999999999999999 5
./main -m -2147483648 -1

./main -t 0.001 3 4 5
./main -t 0.001 5 3 4
./main -t 0.001 3 4 6
./main -t 0.001 1 2 10
./main -t 0.001 -3 4 5
./main -t 0.001 0 4 5
./main -t 0.001 1e160 1e160 1e160