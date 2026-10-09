#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>



int parse_double(const char *str, double *val) {
    if (str == NULL || val == NULL || str[0] == '\0') {
        return 1;
    }
    char *end = NULL;
    errno = 0;
    double res = strtod(str, &end);
    if (*end != '\0' || errno == ERANGE || isnan(res) || isinf(res)) {
        return 1;
    }
    *val = res;
    return 0;
}


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



int solve_quadratic(double a, double b, double c, double eps, int *count_roots, double *x1, double *x2) {
    if (count_roots == NULL || x1 == NULL || x2 == NULL || eps <= 0.0 || isnan(a) || isnan(b) || isnan(c)) {
        return 1;
    }

    // Линейное уравнение (a = 0)
    if (fabs(a) < eps) {
        if (fabs(b) < eps) {
            if (fabs(c) < eps) {
                *count_roots = -1; // Бесконечно много решений
            } else {
                *count_roots = 0;  // Нет решений
            }
        } else {
            *count_roots = 1;
            *x1 = -c / b;
            *x2 = *x1;
        }
        return 0;
    }

    // Вычисление дискриминанта с проверкой на переполнение double
    double discr = b * b - 4.0 * a * c;
    if (isinf(discr)) {
        return 2; // Переполнение
    }

    if (discr > eps) {
        *count_roots = 2;
        *x1 = (-b + sqrt(discr)) / (2.0 * a);
        *x2 = (-b - sqrt(discr)) / (2.0 * a);
    } else if (fabs(discr) <= eps) {
        *count_roots = 1;
        *x1 = -b / (2.0 * a);
        *x2 = *x1;
    } else {
        *count_roots = 0; // Нет действительных корней
    }

    return 0;
}

//-q: Решение для 6 перестановок коэффициентов
int flag_q(double eps, double a, double b, double c, 
           double unique_p[6][3], int roots_count[6], double x1_arr[6], double x2_arr[6], int *unique_len) {
    
    if (unique_p == NULL || roots_count == NULL || x1_arr == NULL || x2_arr == NULL || unique_len == NULL || eps <= 0.0) {
        return 1;
    }

    const double p[6][3] = {
        {a, b, c},
        {a, c, b},
        {b, a, c},
        {b, c, a},
        {c, a, b},
        {c, b, a}
    };

    *unique_len = 0;
    for (int i = 0; i < 6; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (fabs(p[i][0] - p[j][0]) < eps &&
                fabs(p[i][1] - p[j][1]) < eps &&
                fabs(p[i][2] - p[j][2]) < eps) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            int idx = *unique_len;
            unique_p[idx][0] = p[i][0];
            unique_p[idx][1] = p[i][1];
            unique_p[idx][2] = p[i][2];

            int st = solve_quadratic(p[i][0], p[i][1], p[i][2], eps, &roots_count[idx], &x1_arr[idx], &x2_arr[idx]);
            if (st != 0) {
                return st;
            }
            (*unique_len)++;
        }
    }
    return 0;
}


//-m: Проверка кратности чисел

int flag_m(int a, int b, int *is_multiple) {
    if (is_multiple == NULL || b == 0) {
        return 1; // Деление на 0 или некорректный указатель
    }

    // Защита от переполнения / UB при INT_MIN % -1
    if (a == INT_MIN && b == -1) {
        *is_multiple = 1;
        return 0;
    }

    if (a % b == 0) {
        *is_multiple = 1;
    } else {
        *is_multiple = 0;
    }
    return 0;
}


//-t: Проверка прямоугольного треугольника

int flag_t(double eps, double a, double b, double c, int *is_right) {
    if (is_right == NULL || eps <= 0.0) {
        return 1;
    }

    if (a <= eps || b <= eps || c <= eps) {
        *is_right = 0;
        return 0;
    }

    double max_side = a;
    double s1 = b;
    double s2 = c;

    if (b > max_side) {
        max_side = b;
        s1 = a;
        s2 = c;
    }
    if (c > max_side) {
        max_side = c;
        s1 = a;
        s2 = b;
    }

    // Неравенство треугольника
    if ((s1 + s2) - max_side <= eps) {
        *is_right = 0;
        return 0;
    }

    // Защита от переполнения
    if (max_side > 1e150 || s1 > 1e150 || s2 > 1e150) {
        return 2; // Переполнение
    }

    if (fabs((s1 * s1 + s2 * s2) - (max_side * max_side)) < eps) {
        *is_right = 1;
    } else {
        *is_right = 0;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Ошибка - Отсутствует аргумент флага.\n");
        return 1;
    }

    const char *flag_str = argv[1];
    if ((flag_str[0] != '-' && flag_str[0] != '/') || flag_str[1] == '\0' || flag_str[2] != '\0') {
        printf("Ошибка - Флаг должен начинаться с '-' или '/' и состоять из одного символа.\n");
        return 1;
    }

    char flag = flag_str[1];

    if (flag == 'q') {
        if (argc != 6) {
            printf("Ошибка - Флаг -q требует 4 параметра: <эпсилон> <a> <b> <c>\n");
            return 1;
        }
        double eps, a, b, c;
        if (parse_double(argv[2], &eps) != 0 || eps <= 0.0 || eps >= 1.0) {
            printf("Ошибка - Некорректный параметр эпсилон.\n");
            return 1;
        }
        if (parse_double(argv[3], &a) != 0 || parse_double(argv[4], &b) != 0 || parse_double(argv[5], &c) != 0) {
            printf("Ошибка - Некорректные параметры коэффициентов.\n");
            return 1;
        }

        double unique_p[6][3];
        int roots_count[6];
        double x1_arr[6];
        double x2_arr[6];
        int unique_len = 0;

        int st = flag_q(eps, a, b, c, unique_p, roots_count, x1_arr, x2_arr, &unique_len);
        if (st == 0) {
            printf("=== Решения для уникальных перестановок ===\n");
            for (int i = 0; i < unique_len; i++) {
                printf("Коэффициенты: a = %g, b = %g, c = %g -> ", unique_p[i][0], unique_p[i][1], unique_p[i][2]);
                if (roots_count[i] == -1) {
                    printf("Бесконечно много решений\n");
                } else if (roots_count[i] == 0) {
                    printf("Нет решений\n");
                } else if (roots_count[i] == 1) {
                    printf("Один корень: x = %g\n", x1_arr[i]);
                } else {
                    printf("Два корня: x1 = %g, x2 = %g\n", x1_arr[i], x2_arr[i]);
                }
            }
        } else if (st == 2) {
            printf("Ошибка - Произошло переполнение при вычислениях.\n");
            return 1;
        } else {
            printf("Ошибка - Неверные параметры.\n");
            return 1;
        }

    } else if (flag == 'm') {
        if (argc != 4) {
            printf("Ошибка - Флаг -m требует 2 параметра: <число1> <число2>\n");
            return 1;
        }
        int num1, num2;
        if (parse_int(argv[2], &num1) != 0 || parse_int(argv[3], &num2) != 0) {
            printf("Ошибка - Некорректные целые параметры.\n");
            return 1;
        }

        int is_multiple = 0;
        int st = flag_m(num1, num2, &is_multiple);
        if (st == 0) {
            if (is_multiple) {
                printf("Число %d КРАТНО числу %d\n", num1, num2);
            } else {
                printf("Число %d НЕ КРАТНО числу %d\n", num1, num2);
            }
        } else {
            printf("Ошибка - Деление на ноль или переданы неверные параметры.\n");
            return 1;
        }

    } else if (flag == 't') {
        if (argc != 6) {
            printf("Ошибка - Флаг -t требует 4 параметра: <эпсилон> <сторона1> <сторона2> <сторона3>\n");
            return 1;
        }
        double eps, a, b, c;
        if (parse_double(argv[2], &eps) != 0 || eps <= 0.0 || eps >= 1.0) {
            printf("Ошибка - Некорректный параметр эпсилон.\n");
            return 1;
        }
        if (parse_double(argv[3], &a) != 0 || parse_double(argv[4], &b) != 0 || parse_double(argv[5], &c) != 0) {
            printf("Ошибка - Некорректные параметры длин сторон.\n");
            return 1;
        }

        int is_right = 0;
        int st = flag_t(eps, a, b, c, &is_right);
        if (st == 0) {
            if (is_right) {
                printf("Стороны %g, %g, %g МОГУТ являться сторонами прямоугольного треугольника.\n", a, b, c);
            } else {
                printf("Стороны %g, %g, %g НЕ МОГУТ являться сторонами прямоугольного треугольника.\n", a, b, c);
            }
        } else if (st == 2) {
            printf("Ошибка - Переполнение при вычислениях (стороны слишком велики).\n");
            return 1;
        } else {
            printf("Ошибка - Переданы некорректные параметры.\n");
            return 1;
        }

    } else {
        printf("Ошибка: Неизвестный флаг '%s'.\n", flag_str);
        return 1;
    }

    return 0;
}
