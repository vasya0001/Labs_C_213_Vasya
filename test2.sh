#!/bin/bash

gcc -std=c99 -Wall -Wextra task2sem3.c -lm -o main || exit 1

./main
./main 0.001 extra
./main abc
./main -0.001
./main 0.0
./main 1.0
./main 1.5
./main 99999999999999999999999999

./main 0.1
./main 0.01
./main 0.001
./main 0.0001
./main 0.00001
./main 0.000001
./main 1e-7
./main 1e-10
./main 1e-15