#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#define BUFFER_SIZE 256


int parse_int(const char *str, int *val) {
    if (str == NULL || val == NULL || str[0] == '\0') {
        return 1;
    }
    char *end = NULL;
    errno = 0;
    long res = strtol(str, &end, 10);
    if (*end != '\0' || errno == ERANGE || res > INT_MAX || res < INT_MIN) {
        return 1;
    }
    *val = (int)res;
    return 0;
}

int string_to_long(const char *str, int base, long long *val) {
    if (str == NULL || val == NULL || base < 2 || base > 36 || str[0] == '\0') {
        return 1;
    }

    int is_negative = 0;
    int start_idx = 0;

    if (str[0] == '-') {
        is_negative = 1;
        start_idx = 1;
    } else if (str[0] == '+') {
        start_idx = 1;
    }

    if (str[start_idx] == '\0') {
        return 1;
    }

    // Пропуск ведущих нулей во входной строке
    while (str[start_idx] == '0' && str[start_idx + 1] != '\0') {
        start_idx++;
    }

    unsigned long long result = 0;
    unsigned long long limit = is_negative ? (unsigned long long)LLONG_MAX + 1ULL : (unsigned long long)LLONG_MAX;

    for (int i = start_idx; str[i] != '\0'; i++) {
        char c = str[i];
        int digit = -1;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else {
            return 1; // Недопустимый символ или строчная буква
        }

        if (digit >= base) {
            return 1; // Цифра слишком велика для текущего основания
        }

        // Проверка переполнения умножения и сложения
        if (result > (limit - digit) / base) {
            return 2; // Переполнение типа long long
        }

        result = result * base + digit;
    }

    if (is_negative) {
        if (result == (unsigned long long)LLONG_MAX + 1ULL) {
            *val = LLONG_MIN;
        } else {
            *val = -(long long)result;
        }
    } else {
        *val = (long long)result;
    }

    return 0;
}

int long_to_string(long long val, int base, char *out_str, size_t out_size) {
    if (out_str == NULL || out_size == 0 || base < 2 || base > 36) {
        return 1;
    }

    if (val == 0) {
        if (out_size < 2) return 1;
        out_str[0] = '0';
        out_str[1] = '\0';
        return 0;
    }

    int is_negative = 0;
    unsigned long long abs_val;
    if (val < 0) {
        is_negative = 1;
        abs_val = (unsigned long long)(-(val + 1)) + 1ULL; // Защита от overflow при LLONG_MIN
    } else {
        abs_val = (unsigned long long)val;
    }

    char temp[128];
    int len = 0;

    while (abs_val > 0) {
        int rem = (int)(abs_val % base);
        if (rem < 10) {
            temp[len++] = '0' + rem;
        } else {
            temp[len++] = 'A' + (rem - 10);
        }
        abs_val /= base;
    }

    if (is_negative) {
        temp[len++] = '-';
    }

    if ((size_t)len >= out_size) {
        return 1;
    }

    for (int i = 0; i < len; i++) {
        out_str[i] = temp[len - 1 - i];
    }
    out_str[len] = '\0';

    return 0;
}


unsigned long long safe_abs(long long val) {
    if (val < 0) {
        return (unsigned long long)(-(val + 1)) + 1ULL;
    }
    return (unsigned long long)val;
}

int print_number_in_bases(long long num, const char *label) {
    int bases[] = {9, 18, 27, 36};
    char buffer[BUFFER_SIZE];

    printf("\n=== %s ===\n", label);
    printf("Значение в 10-й СС: %lld\n", num);

    for (int i = 0; i < 4; i++) {
        if (long_to_string(num, bases[i], buffer, sizeof(buffer)) == 0) {
            printf("Основание %2d: %s\n", bases[i], buffer);
        } else {
            printf("Ошибка перевода в основание %d\n", bases[i]);
            return 1;
        }
    }
    return 0;
}

// -------------------------------------------------------------
// Обработка потока ввода чисел
// -------------------------------------------------------------
int process_input_stream(int base) {
    char input[BUFFER_SIZE];
    long long sum = 0;
    unsigned long long max_abs_val = 0;
    long long max_abs_num = 0;
    int count = 0;

    printf("Вводите числа в системе счисления с основанием %d (для завершения введите 'Stop'):\n", base);

    while (1) {
        printf("> ");
        if (scanf("%255s", input) != 1) {
            // Прерываем ввод при EOF
            break;
        }

        if (strcmp(input, "Stop") == 0) {
            break;
        }

        long long current_val = 0;
        int st = string_to_long(input, base, &current_val);
        if (st != 0) {
            printf("Ошибка: Некорректное число или переполнение '%s' для СС с основанием %d.\n", input, base);
            continue;
        }

        // Проверка переполнения при суммировании
        if ((current_val > 0 && sum > LLONG_MAX - current_val) ||
            (current_val < 0 && sum < LLONG_MIN - current_val)) {
            printf("Ошибка: Переполнение при вычислении суммы всех чисел.\n");
            return 1;
        }

        sum += current_val;
        count++;

        unsigned long long current_abs = safe_abs(current_val);
        if (count == 1 || current_abs > max_abs_val) {
            max_abs_val = current_abs;
            max_abs_num = current_val;
        }
    }

    if (count == 0) {
        printf("\nЧисла не были введены.\n");
        return 0;
    }

    if (print_number_in_bases(max_abs_num, "Максимальное по модулю число") != 0) {
        return 1;
    }

    if (print_number_in_bases(sum, "Сумма всех введенных чисел") != 0) {
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    int base = 0;

    if (argc >= 2) {
        if (parse_int(argv[1], &base) != 0 || base < 2 || base > 36) {
            printf("Ошибка: Основание системы счисления должно быть целым числом от 2 до 36.\n");
            return 1;
        }
    } else {
        printf("Введите основание системы счисления [2..36]: ");
        char input[BUFFER_SIZE];
        if (scanf("%255s", input) != 1 || parse_int(input, &base) != 0 || base < 2 || base > 36) {
            printf("Ошибка: Некорректное основание системы счисления.\n");
            return 1;
        }
    }

    if (process_input_stream(base) != 0) {
        printf("Ошибка при обработке чисел.\n");
        return 1;
    }

    return 0;
}
