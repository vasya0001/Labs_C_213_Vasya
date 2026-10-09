gcc -std=c99 -Wall -Wextra task1sem3.c -o main || exit 1
#ОШИБКИ ВВОДА И АРГУМЕНТОВ
./main
./main 10
./main 10 -h extra
./main abc -h
./main 99999999999999999999 -h
./main 5 h
./main 5 -hello
./main 5 -z

#ФЛАГ -h
./main 25 -h
./main 20 /h
./main 150 -h
./main 0 -h
./main -5 -h

#ФЛАГ -p
./main 17 -p
./main 4 -p
./main 1 -p
./main 0 -p
./main -5 -p

#ФЛАГ -a
./main 10 -a
./main 1 -a
./main 0 -a
./main -5 -a
./main 2147483647 -a

#ФЛАГ -f
./main 0 -f
./main 5 -f
./main 20 -f
./main 21 -f
./main -3 -f

#ФЛАГ -e
./main 3 -e
./main 10 -e
./main 11 -e
./main 0 -e
./main -2 -e

#ФЛАГ -s 
./main 0 -s
./main 255 -s
./main 4096 -s
./main -10 -s
