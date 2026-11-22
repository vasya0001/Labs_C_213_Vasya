#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef enum{
    STATUS_OK,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_NULL,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_FILE
}Status;

Status str_len(const char *str, size_t *result){
    size_t len = 0;
    while(str[len]!= '\0'){
        len++;
    }
    *result = len;
    return STATUS_OK;
}

Status digit_in_file(const char *filename, const char*outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    int c;
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }
    FILE *out = fopen(outname, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }
}

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("Подано неверное колличество аргументов\n");
        return 1;
    }
}