#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h> 
#include <stdarg.h>   

typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_FILE,   // ДОБАВЛЕНО: ошибка открытия файла
}Status;
Status str_len(const char *str, size_t *result){
    if(str == NULL || result == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t len = 0;
    while(str[len] != '\0'){
        len++;
    }
    *result = len;
    return STATUS_OK;
}




Status parse_size(const char *str, size_t *value) {
    if (str == NULL || value == NULL) {
        return STATUS_ERROR_NULL;
    }
    if (str[0] == '\0') {
        return STATUS_ERROR_VALUE;
    }
    size_t result = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return STATUS_ERROR_VALUE;
        }
        size_t digit = (size_t)(str[i] - '0');
        if (result > (SIZE_MAX - digit) / 10) {
            return STATUS_ERROR_VALUE;   // переполнение
        }
        result = result * 10 + digit;
    }
    *value = result;
    return STATUS_OK;
}

void print_error(Status st){
    switch(st){
        case STATUS_ERROR_NULL:
            fprintf(stderr, "Ошибка: передан нулевой указатель\n");
            break;
        case STATUS_ERROR_VALUE:
            fprintf(stderr, "Ошибка: некорректное значение\n");
            break;
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        case STATUS_ERROR_FILE:   // ДОБАВЛЕНО
            fprintf(stderr, "Ошибка: не удалось открыть файл\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}

typedef struct{
    size_t line; //номер строки
    size_t pos; //номер позиции
}Match;

Status search_file(const char *path, const char *substr, size_t sub_len, Match **matches, size_t *count){
    if(path == NULL || substr == NULL || matches == NULL || count == NULL   ){
        return STATUS_ERROR_NULL;
    }
    if(sub_len == 0){                      
        return STATUS_ERROR_VALUE;
    }
    FILE *file = fopen(path, "r");
    if(file == NULL){
        return STATUS_ERROR_FILE;          
    }
    //читаем весь файл
    char *content = NULL;
    size_t size = 0;
    size_t cap = 0;
    int c;
    while((c = fgetc(file)) != EOF){
        if(size == cap){ //если больше - меняем
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            char *tmp = realloc(content, new_cap * sizeof(*tmp));
            if(tmp == NULL){
                free(content);
                fclose(file);
                return STATUS_ERROR_MEMORY;
            }
            content = tmp;
            cap = new_cap;
        }
        content[size] = (char)c;
        size++;
    }
    fclose(file);
    Match *found = NULL;
    size_t found_count = 0;
    size_t found_cap = 0;
    size_t line = 1;
    size_t pos = 1;
    for(size_t i = 0; i < size; i++){
        if(size - i >= sub_len){              
            size_t j = 0;
            while(j < sub_len && content[i + j] == substr[j]){
                j++;
            }
            if(j == sub_len){                 // совпали все символы подстроки
                if(found_count == found_cap){
                    size_t new_cap = (found_cap == 0) ? 4 : found_cap * 2;
                    Match *tmp = realloc(found, new_cap * sizeof(*tmp));
                    if(tmp == NULL){
                        free(found);
                        free(content);
                        return STATUS_ERROR_MEMORY;
                    }
                    found = tmp;
                    found_cap = new_cap;
                }
                found[found_count].line = line;
                found[found_count].pos = pos;
                found_count++;
            }
        }
        if(content[i] == '\n'){ // сдвигаем счётчики строки и позиции
            line++;
            pos = 1;
        }else{
            pos++;
        }
    }
    free(content);
    *matches = found;
    *count = found_count;
    return STATUS_OK;
}

void print_matches(const char *path, const Match *matches, size_t count){
    printf("Файл: %s\n", path);
    if(count == 0){
        printf("  Вхождений не найдено\n");
        return;
    }
    for(size_t i = 0; i < count; i++){
        printf("  Вхождение: строка %zu, символ %zu\n", matches[i].line, matches[i].pos);
    }
    printf("  Всего вхождений: %zu\n", count);
}

Status find_in_files(const char *substr, size_t file_count, ...){
    if(substr == NULL){
        return STATUS_ERROR_NULL;
    }
    if(file_count == 0){                   
        return STATUS_ERROR_VALUE;
    }
    size_t sub_len = 0;
    Status st = str_len(substr, &sub_len);
    if(st != STATUS_OK){
        return st;
    }
    va_list args;
    va_start(args, file_count);
    for(size_t i = 0; i < file_count; i++){
        const char *path = va_arg(args, char *);   
        Match *matches = NULL;
        size_t count = 0;
        st = search_file(path, substr, sub_len, &matches, &count);
        if(st != STATUS_OK){
            va_end(args);
            return st;
        }
        print_matches(path, matches, count);
        free(matches);
    }
    va_end(args);
    return STATUS_OK;
}

int main(int argc, char *argv[]){
    if(argc < 4){
        fprintf(stderr, "Нужно подать минимум 4 аргумента\n");   
        return 1;
    }
    size_t sub_len = 0;
    Status st = str_len(argv[1], &sub_len);
    if(st != STATUS_OK){
        print_error(st);
        return 1;
    }
    if(sub_len == 0){
        fprintf(stderr, "Ошибка: подстрока не должна быть пустой\n");
        return 1;
    }
    size_t file_count = 0;
    st = parse_size(argv[2], &file_count);
    if(st != STATUS_OK){
        fprintf(stderr, "Ошибка: количество файлов должно быть целым неотрицательным числом\n");
        return 1;
    }
    if(file_count == 0 || file_count != (size_t)(argc - 3)){
        fprintf(stderr, "Ошибка: количество файлов не совпадает с числом переданных путей\n");
        return 1;
    }
    size_t i = 0;
    while(i < file_count){
        size_t left = file_count - i;      // сколько файлов осталось
        if(left >= 3){
            st = find_in_files(argv[1], 3, argv[3 + i], argv[4 + i], argv[5 + i]);
            i += 3;
        }else if(left == 2){
            st = find_in_files(argv[1], 2, argv[3 + i], argv[4 + i]);
            i += 2;
        }else{
            st = find_in_files(argv[1], 1, argv[3 + i]);
            i += 1;
        }
        if(st != STATUS_OK){
            print_error(st);
            return 1;
        }
    }
    return 0;
}