#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_FILE
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


Status char_to_digit(char c, size_t base, size_t *digit){
    if(digit == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t v = 0;
    if(c >= '0' && c <= '9'){
        v = (size_t)(c - '0');     
    }else if(c >= 'A' && c <= 'Z'){
        v = (size_t)(c - 'A') + 10;
    }else if(c >= 'a' && c <= 'z'){          
        v = (size_t)(c - 'a') + 10;
    }else{
        return STATUS_ERROR_VALUE;
    }
    if(v >= base){
        return STATUS_ERROR_VALUE;     
    }
    *digit = v;
    return STATUS_OK;
}

Status to_ten(const char *str, size_t base, unsigned long long *result){
    if(str == NULL || result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(base < 2 || base > 36){
        return STATUS_ERROR_VALUE;
    }
    size_t len = 0;
    Status st = str_len(str, &len);
    if(st != STATUS_OK){
        return st;
    }
    if(len == 0){
        return STATUS_ERROR_VALUE;
    }
    unsigned long long res = 0;
    unsigned long long base_ull = (unsigned long long)base;
    for(size_t i = 0; i < len; i++){
        size_t digit = 0;
        st = char_to_digit(str[i], base, &digit); //иду по числу
        if(st != STATUS_OK){
            return st;
        }
        unsigned long long digit_ull = (unsigned long long)digit;
        if(res > (ULLONG_MAX - digit_ull) / base_ull){ //переполнение
            return STATUS_ERROR_VALUE;
        }
        res = res * base_ull + digit_ull;
    }
    *result = res;
    return STATUS_OK;
}



Status sum_and_max(const char **values, size_t base,size_t count, long long *max, long long *sum){
    if(values == NULL || max == NULL || sum == NULL){
        return STATUS_ERROR_NULL;
    }
    long long max_val = 0;
    long long summa = 0;
    long long value = 0;
    for(size_t i = 0; i < count;i++){
        Status st = to_ten(values[i], base, &value);
        if(st != STATUS_OK){
            return st;
        }
        if(llabs(value) > llabs(max_val)){
            max_val = value;
        }
        //переволнение
        if (value > 0 && summa > LLONG_MAX - value) {
            return STATUS_ERROR_VALUE;
        }
        if (value < 0 && summa < LLONG_MIN - value) {
            return STATUS_ERROR_VALUE;
        }
        summa += value;
    }
    *max = max_val;
    *sum = summa;
    return STATUS_OK;
}


static char digit_to_char(size_t digit) {
    if (digit < 10) {
        return (char)('0' + digit);
    }
    return (char)('A' + (digit - 10));
}


Status print_in_base(long long val, size_t base) {
    if (base < 2 || base > 36) {
        return STATUS_ERROR_VALUE;
    }
    unsigned long long uval = 0;
    if (val < 0) {
        printf("-");
        if (val == LLONG_MIN) {
            uval = (unsigned long long)LLONG_MAX + 1;
        } else {
            uval = (unsigned long long)llabs(val);
        }
    } else {
        uval = (unsigned long long)val;
    }

    if (uval == 0) {
        printf("0");
        return STATUS_OK;
    }

    char buf[65];
    size_t pos = 0;
    while (uval > 0) {
        size_t rem = (size_t)(uval % base);
        buf[pos++] = digit_to_char(rem);
        uval /= base;
    }
    for (size_t i = pos; i > 0; i--) {
        printf("%c", buf[i - 1]);
    }
    return STATUS_OK;
}


Status to_systems(const char **values, size_t base, size_t count) {
    if (values == NULL) {
        return STATUS_ERROR_NULL;
    }

    long long max = 0;
    long long sum = 0;
    Status st = sum_and_max(values, base, count, &max, &sum);
    if (st != STATUS_OK) {
        return st;
    }

    const size_t bases[] = {9, 18, 27, 36};
    const size_t bases_count = sizeof(bases) / sizeof(bases[0]);

    printf("Max abs: ");
    st = print_in_base(max, 10);
    if (st != STATUS_OK){
         return st;
    }
    printf("\n");
    for (size_t i = 0; i < bases_count; i++) {
        printf("  base %zu: ", bases[i]);
        st = print_in_base(max, bases[i]);
        if (st != STATUS_OK) return st;
        printf("\n");
    }

    printf("Sum: ");
    st = print_in_base(sum, 10);
    if (st != STATUS_OK){
        return st;
    }
    printf("\n");
    for (size_t i = 0; i < bases_count; i++) {
        printf("  base %zu: ", bases[i]);
        st = print_in_base(sum, bases[i]);
        if (st != STATUS_OK) return st;
        printf("\n");
    }
    return STATUS_OK;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Недостаточно аргументов!\n");
        return 1;
    }
    long long base = 0;
    Status st_base = to_ten(argv[1], 10, &base);
    if (st_base != STATUS_OK || base < 2 || base > 36) {
        printf("Некорректное основание!\n");
        return 1;
    }
    size_t base_t = (size_t)base;

    int stop = 0;
    size_t count = 0;
    for (int i = 2; i < argc; i++) { //чисел передано до стоп  с argv[2]
        if (strcmp(argv[i], "Stop") == 0) {
            stop =1;
            break;
        }
        count++;
    }
    if(stop == 0){
        printf("Нету завершающего аргумента stop\n");
        return 1;
    }
    if (count == 0) {
        printf("Не передано ни одного числа до Stop!\n");
        return 1;
    }
    const char **values = (const char **)(argv + 2); //массив в функцию
    Status res = to_systems(values, base_t, count);
    if (res != STATUS_OK) {
        printf("Ошибка при обработке чисел (код %d)!\n", (int)res);
        return 1;
    }

    return 0;
}



