#!/bin/bash

gcc -std=c99 -Wall -Wextra task5sem3.c -lm -o main || exit 1
./main
./main 0.001
./main abc 1
./main 0.001 abc
./main -0.01 1 0.1
./main 0.0 1 0.1
./main 1.5 1 0.1
./main 0.001 0 0.1
./main 0.001 9 0.1
./main 0.001 1

./main 0.001 1 0.5
./main 0.001 1 -1.0
./main 0.001 2 0.5
./main 0.001 2 3.14
./main 0.001 3 0.2
./main 0.001 3 0.5
./main 0.001 3 1.0
./main 0.001 4 0.5
./main 0.001 4 0.99
./main 0.001 4 1.0
./main 0.001 4 1.5

./main 0.001 5
./main 0.001 6
./main 0.001 7
./main 0.001 8

./main 1e-7 1 0.1
./main 1e-10 6
./main 1e-15 8
