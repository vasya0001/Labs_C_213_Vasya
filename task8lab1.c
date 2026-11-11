#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef enum{
    STATUS_OK,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_FILE
}Status;

Status char_to_digit(char c, size_t *digit){
    if(digit == NULL){
        return STATUS_ERROR_NULL;
    }
    if(c >= '0' && c <= '9'){
        *digit = (size_t)(c - '0');
    }
    else if(c>='A'&&c<='Z'){
        *digit = (size_t)(c - 'A') + 10;
    }
    else if(c >= 'a' && c <= 'z'){
        *digit = (size_t)(c - 'a') + 10;
    }
    else{
        return STATUS_ERROR_VALUE;
    }
    return STATUS_OK;
}

Status process(const char *tok, size_t len, FILE *out){
    if(tok == NULL || out == NULL){
        return STATUS_ERROR_NULL;
    }
    if(len == 0){
        return STATUS_ERROR_VALUE;
    }
    size_t max = 0;
    for(size_t i = 0; i< len; i++){
        size_t v = 0;
        Status st = char_to_digit(tok[i], &v);
        if(st != STATUS_OK){
            return st;
        }
        if(v > max){
            max = v;
        }
    }
    size_t base = max + 1;
    if(base < 2){
        base = 2;
    }
    unsigned long long value = 0;
    for(size_t i = 0; i < len ;i++){
        size_t d = 0;
        char_to_digit(tok[i], &d);
        if(value > (ULLONG_MAX - d) / base){
            return STATUS_ERROR_VALUE;
        }
        value = value * base +d;
    }
    size_t start = 0;
    while(start + 1 < len && tok[start] == '0'){
        start++;
    }
    for(size_t i = start; i < len; i++){
        fputc(tok[i], out);
    }   
    fprintf(out, " %zu %llu\n", base, value); // число - основание - десятичная форама
    return STATUS_OK;
}
Status go_to_file(const char *filename, const char *outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }
    FILE *out = fopen(outname, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }
    char *word = NULL; //текущее слоао
    size_t len = 0; //len
    size_t mem = 0; //место
    Status st = STATUS_OK;
    int c;
    while((c = fgetc(in)) != EOF){
        if(c == ' ' || c == '\t' || c == '\n'|| c == '\r'){
            if(len > 0){
                st = process(word, len, out);
                if(st != STATUS_OK){
                    break;
                }
                len = 0;
            }
        }else{
            if(len == mem){
                size_t new_mem = (mem == 0)?16 : mem *2;
                char *new_word = realloc(word, new_mem*sizeof(*new_word));
                if(new_word == NULL){
                    st = STATUS_ERROR_MEMORY;
                    break;
                }
                mem = new_mem;
                word = new_word;
            }
            word[len] = (char)c;
            len++;
        }
    }
    if(st == STATUS_OK && len > 0){
        st = process(word, len, out);
    }
    free(word);
    fclose(in);
    fclose(out);
    return st;
}

void print_error(Status st){
    switch(st){
        case STATUS_ERROR_NULL:
            fprintf(stderr, "Ошибка: передан нулевой указатель\n");
            break;
        case STATUS_ERROR_VALUE:
            fprintf(stderr, "Ошибка: недопустимый символ в числе или число слишком большое\n");
            break;
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        case STATUS_ERROR_FILE:
            fprintf(stderr, "Ошибка: не удалось открыть файл\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}


int main(int argc, char *argv[]){ // /task - file - out
    if(argc != 3){
        printf("Подано некорректное колличество аргументов!\n");
        return 1;
    }
    Status st = go_to_file(argv[1], argv[2]);
    if(st != STATUS_OK){
        print_error(st);
        return 1;
    }
    return 0;
}
