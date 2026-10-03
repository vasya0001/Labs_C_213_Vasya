#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
0 - окей
1 - ошибка параметров
2 - нет результата
*/

//чтение числа
int parse(const char *str, int *result){
    if(str == NULL || result == NULL || str[0] == '\0'){
        return 1;
    }
    char *end = NULL;
    long val = strtol(str, &end, 10);

    if (*end != '\0') {
        return 1; // при обнаружении не десятичных символов - ошибка
    }
    *result = (int)val; 
    return 0;

}


// 1.1

int multiple(int x, int *arr, int *count){
    if(x <= 0 || arr == NULL || count == NULL){
        return 1;
    }
    *count = 0;
    for(int i = x; i <= 100; i = i + x){
        arr[*count] = i;
        *count = *count + 1;
    }
    if(*count == 0){
        return 2;
    }
    return 0;
}


//1.2
int is_prime(int x, int *res){
    if(x < 2 || res == NULL){
        return 1;
    }
    *res = 1;
    for(int i = 2; i < x; i++){
        if(x % i == 0){
            *res = 0;
            break; //у числа есть делитель - составное
        }
    }
    return 0;
}


//1.5
int sum_all(int x, long long *result){
    if(x < 1 || result == NULL){
        return 1;
    }
    long long sum = 0;
    for(int i = 1; i <= x; i++){
        sum = sum + i;
    }
    *result = sum;
    return 0;
}


//1.6
int factorial(int x, long long *fac){
    if(x < 0 || fac == NULL){
        return 1;
    }
    *fac = 1;
    for(int i = 1; i <= x; i++){
        *fac = *fac * i;
    }
    return 0;
}


//1.4
int power(long long base, int x, long long *result){
    if(x < 0 || result == NULL){
        return 1;
    }
    long long res = 1;
    for(int i = 0; i < x; i++){
        res = res * base;
    }
    *result = res;
    return 0;
}

//1.3
int get_hex_digits(unsigned int x, char *hex_digits, int *len) {
    if (hex_digits == NULL || len == NULL) {
        return 1;
    }

    const char *hex_chars = "0123456789ABCDEF";
    char temp[32];
    int temp_len = 0;

    if (x == 0) {
        temp[temp_len++] = '0';
    } else {
        while (x > 0) {
            temp[temp_len++] = hex_chars[x % 16];
            x /= 16;
        }
    }

    // Переворачиваем цифры от старших к младшим
    for (int i = 0; i < temp_len; i++) {
        hex_digits[i] = temp[temp_len - 1 - i];
    }
    hex_digits[temp_len] = '\0';
    *len = temp_len;

    return 0;
}

int main(int argc, char *argv[]){
    if (argc != 3) { // не передано нужное количество элементов - ошибка
        printf("Ошибка - нужно передать 2 аргумента: число и флаг\n");
        return 1;
    }
    //обработка числа
    int x = 0;
    int st = parse(argv[1], &x); // перевод второго элемента в int
    if (st != 0) {
        printf("Ошибка - первый аргумент должен быть целым числом\n"); //был выявлен не десятичный символ
        return 1;
    }
    //обработка флага
    const char *flag = argv[2];
    if ((flag[0] != '-' && flag[0] != '/') || strlen(flag) != 2) { //проверка флага
        printf("Ошибка - флаг должен начинаться с - или /, иметь длину 2 символа\n");
        return 1;
    }
    char option = flag[1];
    switch(option){


        //1.1
        case 'h':{
            int numbers[100];
            int count = 0;
            st = multiple(x, numbers, &count);
            if(st == 1){
                printf("Ошибка - передайте правильно параметры!\n");
            }
            else if(st == 2){ // если *count - 0, то нет результата, тк в массив ничего не добавится
                printf("Таких чисел нет!\n");
            }
            else{
                printf("Числа, кратные %d: ", x);
                for (int i = 0; i < count; i++) {
                    printf("%d ", numbers[i]);
                }
                printf("\n");
            }
            break; 
        }


        //1.2
        case 'p':{
            int res = 0;
            st = is_prime(x, &res);
            if(st == 1){
                printf("Ошибка - передайте правильно параметры!\n");
            }else{
                if(res == 1){
                    printf("Число простое!\n");
                }
                else{
                    printf("Число составное!\n");
                }
            }
            break;
        }


        //1.5
        case 'a':{
            long long sum = 0;
            st = sum_all(x, &sum);
            if(st == 1){
                printf("Ошибка - передайте правильно параметры!\n");
            }else{
                printf("Сумма всех чисел от 1 до %d : %lld\n", x, sum);
            }
            break;
        }


        //1.6
        case 'f':{
            long long fac = 0;
            st = factorial(x, &fac);
            if(st == 1){
                printf("Ошибка - передайте правильно параметры!\n");
            }else{
                printf("Факториал числа %d равен %lld\n", x, fac);
            }
            break;
        }


        //1.4
        case 'e':{
            if (x < 1 || x > 10) {
                printf("Ошибка - x должно быть от 1 до 10 для флага e\n");
                break;
            }
            printf("Таблица степеней:\n");
            for (int base = 1; base <= 10; base++) { //цикл от 1 до 10
                printf("Основание %d: ", base);
                for (int exp = 1; exp <= x; exp++) { //внутренний цикл от 1 до x
                    long long pow_val = 0;
                    int pow_st = power(base, exp, &pow_val);
                    if(pow_st == 0){
                        printf("%lld ", pow_val);
                    }
                }
                printf("\n");
            }
            break;
        }
        //1.3
        case 's': {
            if (x < 0) {
                printf("Ошибка - число x должно быть неотрицательным.\n");
                break;
            }
            char hex_digits[32];
            int len = 0;
            st = get_hex_digits((unsigned int)x, hex_digits, &len);

            if (st == 0) {
                printf("16-ричные цифры: ");
                for (int i = 0; i < len; i++) {
                    printf("%c ", hex_digits[i]);
                }
                printf("\n");
            }
            break;
        }
        default:
            printf("Ошибка - неизвестный флаг %s\n", flag);
            break;
    }
    return 0;
}
