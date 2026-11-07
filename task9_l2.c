#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_STEPS 30

typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY,
}Status;

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

Status parse_double(const char *str, double *value){
    if(str == NULL || value == NULL){
        return STATUS_ERROR_NULL;
    }
    double result = 0.0;
    size_t i = 0;
    size_t digits = 0;
    while(str[i] >= '0' && str[i] <= '9'){
        result = result * 10 + (double)(str[i] - '0');
        if(result > 1e300){
            return STATUS_ERROR_VALUE;
        }
        digits++;
        i++;
    }
    if(str[i] == '.'){
        i++;
        double scale = 0.1;
        while(str[i] >= '0' && str[i] <= '9'){
            result += (double)(str[i] - '0') * scale;
            scale /= 10;
            digits++;
            i++;
        }
    }
    if(digits == 0 || str[i] != '\0'){
        return STATUS_ERROR_VALUE;       // пусто или посторонние символы
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
            fprintf(stderr, "Ошибка: некорректное значение (дробь должна быть в (0; 1))\n");
            break;
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}

// проверка одной дроби: конечная ли она в системе с основанием base
Status is_finite_fraction(double x, int base, double eps, int *result){
    if(result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(base < 2 || base > 36){
        return STATUS_ERROR_VALUE;
    }
    if(eps <= 0.0 || eps >= 0.5){
        return STATUS_ERROR_VALUE;
    }
    if(x < eps || x > 1.0 - eps){           
        return STATUS_ERROR_VALUE;
    }
    for(int step = 0; step < MAX_STEPS; step++){
        x = x * (double)base;
        x = x - (double)(int)x;             
        if(x < eps || x > 1.0 - eps){       // дробная часть стала нулём
            *result = 1;
            return STATUS_OK;
        }
    }
    *result = 0;                            // нуля не дождались
    return STATUS_OK;
}

Status check_fractions(int base, double eps, int *results, size_t count, ...){
    if(results == NULL){
        return STATUS_ERROR_NULL;
    }
    if(count == 0){
        return STATUS_ERROR_VALUE;
    }
    va_list args;
    va_start(args, count);
    for(size_t i = 0; i < count; i++){
        double x = va_arg(args, double);
        Status st = is_finite_fraction(x, base, eps, &results[i]);
        if(st != STATUS_OK){
            va_end(args);
            return st;
        }
    }
    va_end(args);
    return STATUS_OK;
}

void print_results(int base, const double *values, const int *results, size_t count){
    printf("Система счисления с основанием %d:\n", base);
    for(size_t i = 0; i < count; i++){
        if(results[i] == 1){
            printf("  %.10g - конечное представление\n", values[i]);
        }else{
            printf("  %.10g - бесконечное представление\n", values[i]);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc < 4){
        fprintf(stderr, "Использование: %s основание количество дробь1 дробь2 ...\n", argv[0]);
        return 1;
    }
    // основание
    size_t base_size = 0;
    Status st = parse_size(argv[1], &base_size);
    if(st != STATUS_OK || base_size < 2 || base_size > 36){
        fprintf(stderr, "Ошибка: основание должно быть целым числом от 2 до 36\n");
        return 1;
    }
    int base = (int)base_size;
    double eps = 1e-9;

    // количество дробей
    size_t count = 0;
    st = parse_size(argv[2], &count);
    if(st != STATUS_OK || count == 0){
        fprintf(stderr, "Ошибка: количество должно быть целым числом больше нуля\n");
        return 1;
    }
    if(count != (size_t)(argc - 3)){
        fprintf(stderr, "Ошибка: количество не совпадает с числом переданных дробей\n");
        return 1;
    }

    // массивы под дроби и ответы
    double *values = malloc(count * sizeof(*values));
    int *results = malloc(count * sizeof(*results));
    if(values == NULL || results == NULL){
        free(values);
        free(results);
        print_error(STATUS_ERROR_MEMORY);
        return 1;
    
    for(size_t k = 0; k < count; k++){
        st = parse_double(argv[3 + k], &values[k]);
        if(st != STATUS_OK){
            fprintf(stderr, "Ошибка: \"%s\" не является числом\n", argv[3 + k]);
            free(values);
            free(results);
            return 1;
        }
    }
    size_t i = 0;
    while(i < count){
        size_t left = count - i;
        if(left >= 3){
            st = check_fractions(base, eps, results + i, 3, values[i], values[i + 1], values[i + 2]);
            i += 3;
        }else if(left == 2){
            st = check_fractions(base, eps, results + i, 2, values[i], values[i + 1]);
            i += 2;
        }else{
            st = check_fractions(base, eps, results + i, 1, values[i]);
            i += 1;
        }
        if(st != STATUS_OK){
            print_error(st);
            free(values);
            free(results);
            return 1;
        }
    }

    print_results(base, values, results, count);
    free(values);
    free(results);
    return 0;
}