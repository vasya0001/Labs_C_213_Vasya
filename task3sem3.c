#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int parse_double(const char *str, double *val) {
    if (str == NULL || val == NULL || str[0] == '\0') {
        return 1;
    }
    char *end = NULL;
    double res = strtod(str, &end);
    if (*end != '\0') {
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
    long res = strtol(str, &end, 10);
    if (*end != '\0') {
        return 1;
    }
    *val = (int)res;
    return 0;
}

int solve_quadratic(double a, double b, double c, double eps) {
    printf("Коэффициенты: a = %g, b = %g, c = %g -> ", a, b, c);
    
    if (fabs(a) < eps) {
        if (fabs(b) < eps) {
            if (fabs(c) < eps) {
                printf("Бесконечно много решений\n");
            } else {
                printf("Нет решений\n");
            }
        } else {
            double x = -c / b;
            printf("Линейное уравнение, корень -  x = %g\n", x);
        }
        return 0;
    }

    double discr = b * b - 4.0 * a * c;

    if (discr > eps) {
        double x1 = (-b + sqrt(discr)) / (2.0 * a);
        double x2 = (-b - sqrt(discr)) / (2.0 * a);
        printf("Два корня: x1 = %g, x2 = %g\n", x1, x2);
    } else if (fabs(discr) <= eps) {
        double x = -b / (2.0 * a);
        printf("Один корень -  x = %g\n", x);
    } else {
        printf("Нет действительных корней!\n");
    }
    return 0;
}

int flag_q(double eps, double a, double b, double c) {
    double p[6][3] = {
        {a, b, c},
        {a, c, b},
        {b, a, c},
        {b, c, a},
        {c, a, b},
        {c, b, a}
    };

    printf("=== Решения для уникальных перестановок ===\n");
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
            solve_quadratic(p[i][0], p[i][1], p[i][2], eps);
        }
    }
    return 0;
}

//кратно ли первое число второму
int flag_m(int a, int b, int *is_multiple) {
    if (is_multiple == NULL || a == 0 || b == 0) {
        return 1;
    }
    if (a % b == 0) {
        *is_multiple = 1;
    } else {
        *is_multiple = 0;
    }
    return 0;
}

int flag_t(double eps, double a, double b, double c, int *right) {
    if (right == NULL || eps <= 0.0) {
        return 1;
    }
    if (a <= eps || b <= eps || c <= eps) {
        *right = 0;
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

    if (fabs((s1 * s1 + s2 * s2) - (max_side * max_side)) < eps) {
        *right = 1;
    } else {
        *right = 0;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Ошибка - Отсутствует аргумент флага.\n");
        return 1;
    }

    char *flag_str = argv[1];
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
        flag_q(eps, a, b, c);

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
        if (num1 == 0 || num2 == 0) {
            printf("Ошибка - Параметры для флага -m должны быть ненулевыми целыми числами.\n");
            return 1;
        }
        int is_multiple = 0;
        if (flag_m(num1, num2, &is_multiple) == 0) {
            if (is_multiple) {
                printf("Число %d КРАТНО числу %d\n", num1, num2);
            } else {
                printf("Число %d НЕ КРАТНО числу %d\n", num1, num2);
            }
        } else {
            printf("Ошибка - Не удалось выполнить проверку кратностей.\n");
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
        if (flag_t(eps, a, b, c, &is_right) == 0) {
            if (is_right) {
                printf("Стороны %g, %g, %g МОГУТ являться сторонами прямоугольного треугольника.\n", a, b, c);
            } else {
                printf("Стороны %g, %g, %g НЕ МОГУТ являться сторонами прямоугольного треугольника.\n", a, b, c);
            }
        } else {
            printf("Ошибка: Не удалось выполнить проверку треугольника.\n");
            return 1;
        }

    } else {
        printf("Ошибка: Неизвестный флаг '%s'.\n", flag_str);
        return 1;
    }

    return 0;
}
