#!/bin/bash

gcc -std=c99 -Wall -Wextra task9sem3.c -o main || exit 1

./main
./main abc
./main 0
./main 3

./main 1
./main 1 10
./main 1 abc 10
./main 1 10 abc
./main 1 9999999999999999999 10

./main 1 1 100
./main 1 100 1
./main 1 -50 50
./main 1 -100 -10
./main 1 5 5
./main 1 -2147483648 2147483647

./main 2
./main 2 extra_arg