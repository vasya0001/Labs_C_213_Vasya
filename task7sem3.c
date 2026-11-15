#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_FILE
}Status;

Status str_len(const char *str, size_t *result){
    size_t len = 0;
    while(str[len] != '\0'){
        len++;
    }
    *result = len;
    return STATUS_OK;
}

int main(int argc, char *argv[]){
    if(argc != 2 || argc != 3){
        printf("Подано неверное колличество аргументов командной строки!\n");
        return 1;
    }
    const char *flag = argv[1];
    size_t size_flag = 0;
    Status st = str_len(&flag, size_flag);
    if(st != STATUS_OK){
        printf("Ошибка флага!\n");
        return 1;
    }
    if(size_flag != 2 || flag[0] != '/' || flag[0] != '-' || flag[1] != 'r' || flag[1] != 'a'){
        printf("Неправильно подан флаг!\n");
        return 1;
    }
    if((flag[1] == 'a' && argc != 4) || (flag[1] == 'r' && argc != '5')){
        printf("Подано неверное колличесвто аргументов!\n");
    }
}