#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

#define FIXED_ARRAY_SIZE 15


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

int get_random_int(int min_val, int max_val) {
    if (min_val > max_val) {
        int temp = min_val;
        min_val = max_val;
        max_val = temp;
    }

    unsigned int range = (unsigned int)max_val - (unsigned int)min_val + 1;
    if (range == 0) {
        return min_val; // Крайний случай (все значения типа int)
    }

    return min_val + (int)(rand() % range);
}


int swap_min_max_one_pass(int *arr, int size) {
    if (arr == NULL || size <= 0) {
        return 1;
    }

    int min_idx = 0;
    int max_idx = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }

    int temp = arr[min_idx];
    arr[min_idx] = arr[max_idx];
    arr[max_idx] = temp;

    return 0;
}

int handle_task_1(int a, int b) {
    int arr[FIXED_ARRAY_SIZE];
    
    for (int i = 0; i < FIXED_ARRAY_SIZE; i++) {
        arr[i] = get_random_int(a, b);
    }

    printf("Исходный массив [%d..%d]:\n", a, b);
    for (int i = 0; i < FIXED_ARRAY_SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    if (swap_min_max_one_pass(arr, FIXED_ARRAY_SIZE) != 0) {
        return 1;
    }

    printf("Массив после замены min и max местами:\n");
    for (int i = 0; i < FIXED_ARRAY_SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    return 0;
}


int find_closest(int val, const int *B, int size_B, int *closest_val) {
    if (B == NULL || closest_val == NULL || size_B <= 0) {
        return 1;
    }

    int closest = B[0];
    long long min_diff = llabs((long long)val - (long long)B[0]);

    for (int i = 1; i < size_B; i++) {
        long long diff = llabs((long long)val - (long long)B[i]);
        if (diff < min_diff) {
            min_diff = diff;
            closest = B[i];
        }
    }

    *closest_val = closest;
    return 0;
}


int build_array_c(const int *A, int size_A, const int *B, int size_B, int **C) {
    if (A == NULL || B == NULL || C == NULL || size_A <= 0 || size_B <= 0) {
        return 1;
    }

    int *arr_C = (int *)malloc(size_A * sizeof(int));
    if (arr_C == NULL) {
        return 2; // Ошибка выделения памяти
    }

    for (int i = 0; i < size_A; i++) {
        int closest_b = 0;
        if (find_closest(A[i], B, size_B, &closest_b) != 0) {
            free(arr_C);
            return 1;
        }
        
        // Проверка на переполнение при сложении A[i] + closest_b
        long long sum = (long long)A[i] + (long long)closest_b;
        if (sum > INT_MAX || sum < INT_MIN) {
            free(arr_C);
            return 2; // Вычислительная ошибка
        }
        
        arr_C[i] = (int)sum;
    }

    *C = arr_C;
    return 0;
}

int handle_task_2(void) {
    int size_A = get_random_int(10, 10000);
    int size_B = get_random_int(10, 10000);

    int *A = (int *)malloc(size_A * sizeof(int));
    int *B = (int *)malloc(size_B * sizeof(int));

    if (A == NULL || B == NULL) {
        free(A);
        free(B);
        return 2;
    }

    for (int i = 0; i < size_A; i++) {
        A[i] = get_random_int(-1000, 1000);
    }
    for (int i = 0; i < size_B; i++) {
        B[i] = get_random_int(-1000, 1000);
    }

    int *C = NULL;
    int st = build_array_c(A, size_A, B, size_B, &C);
    if (st != 0) {
        free(A);
        free(B);
        return st;
    }

    printf("Размер массива A: %d\n", size_A);
    printf("Размер массива B: %d\n", size_B);
    printf("Размер массива C: %d\n\n", size_A);

    printf("Первые 10 элементов массива A:\n");
    for (int i = 0; i < (size_A < 10 ? size_A : 10); i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Первые 10 элементов массива B:\n");
    for (int i = 0; i < (size_B < 10 ? size_B : 10); i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("Первые 10 элементов массива C (C[i] = A[i] + closest(B)):\n");
    for (int i = 0; i < (size_A < 10 ? size_A : 10); i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    free(A);
    free(B);
    free(C);
    return 0;
}


int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    if (argc < 2) {
        printf("Ошибка: Недостаточно аргументов.\n");
        printf("Использование: %s <номер_задачи_1-2> [a] [b]\n", argv[0]);
        return 1;
    }

    int task = 0;
    if (parse_int(argv[1], &task) != 0) {
        printf("Ошибка: Некорректный номер задачи.\n");
        return 1;
    }

    switch (task) {
        case 1: {
            if (argc < 4) {
                printf("Ошибка: Для задачи 1 передайте границы диапазона a и b.\n");
                return 1;
            }
            int a, b;
            if (parse_int(argv[2], &a) != 0 || parse_int(argv[3], &b) != 0) {
                printf("Ошибка: Некорректные целочисленные параметры a и b.\n");
                return 1;
            }
            if (handle_task_1(a, b) != 0) {
                printf("Ошибка выполнения задачи 1.\n");
                return 1;
            }
            break;
        }
        case 2: {
            if (handle_task_2() != 0) {
                printf("Ошибка выполнения задачи 2 (не удалось выделить память или произошло переполнение).\n");
                return 1;
            }
            break;
        }
        default:
            printf("Ошибка: Неверный номер задачи (доступны 1 и 2).\n");
            return 1;
    }

    return 0;
}