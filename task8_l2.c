#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h> 
#include <stdarg.h>   

typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_FILE,
    STATUS_ERROR_MEMORY
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

Status parse_firstTwo(const char *str, size_t *result){
    if(str == NULL ||result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(str[0] == '\0'){
        return STATUS_ERROR_VALUE;
    }
    size_t value = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return STATUS_ERROR_VALUE;
        }
        size_t digit = (size_t)(str[i] - '0');
        if (value > (SIZE_MAX - digit) / 10) {
            return STATUS_ERROR_VALUE;   // переполнение
        }
        value = value * 10 + digit;
    }
    *result = value;
    return STATUS_OK;
}

void print_error(Status st){
    switch(st){
        case STATUS_ERROR_NULL:
            fprintf(stderr, "Ошибка: передан нулевой указатель\n");
            break;
        case STATUS_ERROR_VALUE:
            fprintf(stderr, "Ошибка: некорректное значение (проверь основание и цифры чисел)\n");
            break;
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}
Status char_to_value(char c, size_t base, size_t *value){
    if(value == NULL){
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
    *value = v;
    return STATUS_OK;
}

Status add_columns(const char *a, const char *b, size_t base, char **result){
    if(a == NULL || b == NULL || result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(base < 2 || base > 36){
        return STATUS_ERROR_VALUE;
    }
    size_t len_a = 0;
    size_t len_b = 0;
    Status st = str_len(a, &len_a);
    if(st != STATUS_OK){
        return st;
    }
    st = str_len(b, &len_b);
    if(st != STATUS_OK){
        return st;
    }
    if(len_a == 0 || len_b == 0){
        return STATUS_ERROR_VALUE;
    }
    size_t max = (len_a > len_b) ? len_a : len_b;
    if(max > SIZE_MAX - 2){
        return STATUS_ERROR_VALUE;
    }

    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *res = malloc((max + 2) * sizeof(*res));
    if(res == NULL){
        return STATUS_ERROR_MEMORY;
    }

    size_t carry = 0;
    for(size_t i = 0; i < max; i++){
        size_t da = 0;
        size_t db = 0;
        if(i < len_a){
            st = char_to_value(a[len_a - 1 - i], base, &da);
            if(st != STATUS_OK){
                free(res);
                return st;
            }
        }
        if(i < len_b){
            st = char_to_value(b[len_b - 1 - i], base, &db);
            if(st != STATUS_OK){
                free(res);
                return st;
            }
        }
        size_t sum = da + db + carry;
        res[i] = digits[sum % base];
        carry = sum / base;
    }
    size_t n = max;
    if(carry > 0){
        res[n] = digits[carry];
        n++;
    }
    while(n > 1 && res[n - 1] == '0'){      // ведущие нули (пока строка задом наперёд)
        n--;
    }
    res[n] = '\0';

    for(size_t k = 0; k < n / 2; k++){      // разворот
        char tmp = res[k];
        res[k] = res[n - 1 - k];
        res[n - 1 - k] = tmp;
    }
    *result = res;
    return STATUS_OK;
}
Status sum_numbers(size_t base, char **result, size_t count, ...){
    if(result == NULL){
        return STATUS_ERROR_NULL;
    }
    if(base < 2 || base > 36 || count == 0){
        return STATUS_ERROR_VALUE;
    }
    char *cur = NULL; //накопленная сумма
    va_list args;
    va_start(args, count);
    for(size_t i = 0; i < count; i++){
        char *s = va_arg(args, char *);
        char *next = NULL;
        Status st;
        if(i == 0){
            st = add_columns(s, "0", base, &next);   // копия без ведущих нулей + проверка цифр
        }else{
            st = add_columns(cur, s, base, &next);
        }
        if(st != STATUS_OK){
            free(cur);
            va_end(args);
            return st;
        }
        free(cur);
        cur = next;
    }
    va_end(args);
    *result = cur;
    return STATUS_OK;
}

int main(int argc, char *argv[]){
    if(argc < 4){
        fprintf(stderr, "Использование: %s основание количество число1 число2 ...\n", argv[0]);
        return 1;
    }
    size_t size_2 = 0;
    Status st = parse_firstTwo(argv[2], &size_2);
    if(st != STATUS_OK || size_2 == 0){
        fprintf(stderr, "Ошибка: количество должно быть целым числом больше нуля\n");
        return 1;
    }
    if((size_t)(argc - 3) != size_2){
        fprintf(stderr, "Ошибка: количество не совпадает с числом переданных чисел\n");
        return 1;
    }
    size_t size_1 = 0; //основание сс
    st = parse_firstTwo(argv[1], &size_1);
    if(st != STATUS_OK || size_1 < 2 || size_1 > 36){
        fprintf(stderr, "Ошибка: основание должно быть целым числом от 2 до 36\n");
        return 1;
    }
    // сумма копится между вызовами:
    // каждый вызов получает накопленную сумму + одно или два новых числа
    char zero[] = "0";
    char *acc = zero;        // накопленная сумма (сначала "0")
    char *owned = NULL;      // память из malloc; zero освобождать нельзя
    size_t i = 0;
    while(i < size_2){
        char *next = NULL;
        if(size_2 - i >= 2){
            st = sum_numbers(size_1, &next, 3, acc, argv[3 + i], argv[4 + i]);
            i += 2;
        }else{
            st = sum_numbers(size_1, &next, 2, acc, argv[3 + i]);
            i += 1;
        }
        if(st != STATUS_OK){
            print_error(st);
            free(owned);
            return 1;
        }
        free(owned);
        owned = next;
        acc = owned;
    }
    printf("Сумма в системе с основанием %zu: %s\n", size_1, acc);
    free(owned);
    return 0;
}