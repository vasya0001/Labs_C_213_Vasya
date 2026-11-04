#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

typedef enum{
    STATUS_OK,
    STATUS_ERROR_NULL,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_MEMORY
}Status;

Status parse_uint(const char *str, unsigned int *value) {
    if (str == NULL || value == NULL) {
        return STATUS_ERROR_NULL;
    }
    if (str[0] == '\0') {
        return STATUS_ERROR_VALUE;
    }

    unsigned int result = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return STATUS_ERROR_VALUE;
        }
        unsigned int digit = (unsigned int)(str[i] - '0');

        //result * 10 + digit < UINT_MAX
        if (result > (UINT_MAX - digit) / 10) {
            return STATUS_ERROR_VALUE;
        }
        result = result * 10 + digit;
    }
    *value = result;
    return STATUS_OK;
}

//-l
Status str_len(const char *str, size_t *len_str){
    if(str == NULL || len_str == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t len = 0;
    while(str[len] != '\0'){
        len++;
    }
    *len_str = len;
    return STATUS_OK;
}

//-r
Status reversed_string(const char *str, char **reversed){
    if(str == NULL||reversed == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t size = 0;
    Status st = str_len(str, &size);
    if(st != STATUS_OK){
        return st;
    }
    char *rev = malloc((size +1) * sizeof(*rev));
    if (rev == NULL) {
    return STATUS_ERROR_MEMORY;
    }
    for(size_t i = 0; i < size;i++){
        rev[i] = str[size - 1 -i];
    }
    rev[size] = '\0';
    *reversed = rev;
    return STATUS_OK;
}

//-u
Status odd_high(const char *str, char **odd_high_string){
    if(str == NULL || odd_high_string == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t size = 0;
    Status st = str_len(str, &size);
    if(st != STATUS_OK){
        return st;
    }
    char *result = malloc((size + 1) * sizeof(*result));
    if (result == NULL) {
        return STATUS_ERROR_MEMORY;
    }
    for(size_t i = 0; i < size;i++){
        if(i%2 == 1){
            result[i] = (char)toupper((unsigned char)str[i]);
        }else{
            result[i] = str[i];
        }
    }
    result[size] = '\0';
    *odd_high_string = result;
    return STATUS_OK;
}

// -n
Status sort(const char *str, char **sorted){
    if(str == NULL || sorted == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t digits_count = 0;
    size_t letters_count = 0;
    size_t other_count = 0;
    size_t len = 0;
    size_t size = 0;
    Status st = str_len(str, &size);
    if(st != STATUS_OK){
        return st;
    }
        while(str[len] != '\0'){                                  
        if((str[len] >= 'a' && str[len] <= 'z') ||
           (str[len] >= 'A' && str[len] <= 'Z')){            
            letters_count += 1;
        }
        else if(str[len] >= '0' && str[len] <= '9'){          
            digits_count += 1;
        }
        else{                                                 
            other_count += 1;
        }
        len++;                                                
    }
    char *digits = malloc((digits_count + 1) * sizeof(*digits));
    char *letters = malloc((letters_count + 1) * sizeof(*letters));
    char *others = malloc((other_count + 1) * sizeof(*others));
    char *str_res = malloc((digits_count+letters_count+other_count+1) * sizeof(*str_res));
    if(digits == NULL || letters == NULL || others == NULL || str_res == NULL){   
        free(digits);
        free(letters);
        free(others);
        free(str_res);
        return STATUS_ERROR_MEMORY;   
    }
    len = 0;
    letters_count = 0;
    digits_count = 0;
    other_count = 0;
    while(str[len] != '\0'){                                  
        if((str[len] >= 'a' && str[len] <= 'z') ||
           (str[len] >= 'A' && str[len] <= 'Z')){            
            letters[letters_count] = str[len];
            letters_count += 1;
        }
        else if(str[len] >= '0' && str[len] <= '9'){          
            digits[digits_count] = str[len];
            digits_count += 1;
        }
        else{                                                 
            others[other_count] = str[len];
            other_count += 1;
        }
        len++;                                                
    }
    for(int i = 0; i < digits_count; i++){
        str_res[i] = digits[i];
    }
    for(int i = 0; i < letters_count; i++){
        str_res[i+digits_count] = letters[i];
    }
    for(int i = 0; i< other_count;i++){
        str_res[i+digits_count+letters_count] = others[i];
    }
    str_res[digits_count+letters_count+other_count] = '\0';
    *sorted = str_res;
    free(digits);
    free(letters);
    free(others);
    return STATUS_OK;
}




//-с
int get_random_int(int min_val, int max_val) {
    if (min_val > max_val) {
        int temp = min_val;
        min_val = max_val;
        max_val = temp;
    }

    unsigned int range = (unsigned int)max_val - (unsigned int)min_val + 1;
    if (range == 0) {
        return min_val; // Крайний случай 
    }

    return min_val + (int)(rand() % range);
}



Status random_str(int count_arguments,char *argv[],unsigned int seed,char **result){
    if(argv == NULL || result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(count_arguments < 4){
        return STATUS_ERROR_VALUE;
    }
    size_t n = (size_t)(count_arguments - 3);
    int *numbers = malloc((n) * sizeof(*numbers));
    if(numbers == NULL){
        free(numbers);
        return STATUS_ERROR_MEMORY;
    }
    srand(seed);
    size_t len = 0;
    while(len < n){
        int rint = get_random_int(0, (int)n - 1);
        int dupicate = 0;
        for(size_t i = 0; i<len;i++){
            if(numbers[i] == rint){
                dupicate = 1;
                break;
            }
        }
        if(!dupicate){
            numbers[len] = rint;
            len++;
        }
    }
    //длинна каждого
    size_t total = 0;
    for (size_t k = 0; k < n; k++) {
        size_t idx = (size_t)numbers[k];
        const char *s = (idx == 0) ? argv[2] : argv[idx + 3];

        size_t l = 0;
        Status st = str_len(s, &l);
        if (st != STATUS_OK) {
            free(numbers);
            return st;
        }
        if (total > SIZE_MAX - l - 1) {   
            free(numbers);
            return STATUS_ERROR_VALUE;
        }
        total += l; // сложнение в общ
    }

    char *res = malloc((total + 1) * sizeof(*res));
    if (res == NULL) {
        free(numbers);
        return STATUS_ERROR_MEMORY;
    }

    // ручная конкатенация
    size_t pos = 0;
    for (size_t k = 0; k < n; k++) {
        size_t idx = (size_t)numbers[k];
        const char *s = (idx == 0) ? argv[2] : argv[idx + 3];

        for (size_t j = 0; s[j] != '\0'; j++) {
            res[pos] = s[j];
            pos++;
        }
    }
    res[pos] = '\0';

    free(numbers);
    *result = res;
    return STATUS_OK;
}

void print_error(Status st){
    switch(st){
        case STATUS_ERROR_NULL:
            printf("Был передан нулевой указатель!\n");
            break;
        case STATUS_ERROR_VALUE:
            printf("Передано некорректное значение аргумента!\n");
            break;
        case STATUS_ERROR_MEMORY:
            printf("не удалось выделить память!\n");
            break;
        default:
            printf("Неизвестная ошибка!\n");
            break;
    }
}

int main(int argc, char *argv[]){
    if (argc < 3) { // нужны как минимум флаг и строка
        printf("Ошибка - нужно передать флаг и строку\n");
        return 1;
    }

    //обработка флага (без strlen: проверяем символы вручную)
    const char *flag = argv[1];
    if (flag[0] != '-' || flag[1] == '\0' || flag[2] != '\0') {
        printf("Ошибка - флаг должен начинаться с - и иметь длину 2 символа\n");
        return 1;
    }
    char option = flag[1];

    //проверка количества аргументов
    if (option == 'c') {
        if (argc < 4) {
            printf("для -c нужны строка, число и произвольное количество строк\n");
            return 1;
        }
    } else if (argc != 3) {
        printf("для флага %s нужно передать ровно одну строку\n", flag);
        return 1;
    }

    Status st = STATUS_OK;

    switch(option){

        //-l
        case 'l': {
            size_t len = 0;
            st = str_len(argv[2], &len);
            if (st != STATUS_OK) {
                print_error(st);
                return 1;
            }
            printf("Длина строки: %zu\n", len);
            break;
        }

        //-r
        case 'r': {
            char *res = NULL;
            st = reversed_string(argv[2], &res);
            if (st != STATUS_OK) {
                print_error(st);
                return 1;
            }
            printf("Перевёрнутая строка: %s\n", res);
            free(res);
            break;
        }

        //-u
        case 'u': {
            char *res = NULL;
            st = odd_high(argv[2], &res);
            if (st != STATUS_OK) {
                print_error(st);
                return 1;
            }
            printf("Строка с заглавными на нечётных позициях: %s\n", res);
            free(res);
            break;
        }

        //-n
        case 'n': {
            char *res = NULL;
            st = sort(argv[2], &res);
            if (st != STATUS_OK) {
                print_error(st);
                return 1;
            }
            printf("Отсортированная строка: %s\n", res);
            free(res);
            break;
        }

        //-c
        case 'c': {
            unsigned int seed = 0;
            st = parse_uint(argv[3], &seed);
            if (st != STATUS_OK) {
                printf("третий аргумент должен быть целым числом!!!");
                return 1;
            }
            char *res = NULL;
            st = random_str(argc, argv, seed, &res);
            if (st != STATUS_OK) {
                print_error(st);
                return 1;
            }
            printf("Конкатенация в случайном порядке: %s\n", res);
            free(res);
            break;
        }

        default:
            printf("Передан неизвестный флаг %s\n", flag);
            return 1;
    }
    return 0;
}
