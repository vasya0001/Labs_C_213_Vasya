#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

// 1a
int sum_a(double x, double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    double sum = 0.0;
    double term = 1.0;
    int n = 0;

    while (fabs(term) > eps) {
        sum += term;
        n++;
        term *= x / n;
    }
    *res = sum;
    return 0;
}

//1.b
int sum_b(double x, double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    double sum = 0.0;
    double term = 1.0;
    int n = 0;

    while (fabs(term) > eps) {
        sum += term;
        n++;
        term *= -x * x / ((2 * n - 1) * (2 * n));
    }
    *res = sum;
    return 0;
}

//1.c
int sum_c(double x, double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    double sum = 0.0;
    double term = 1.0;
    int n = 0;

    while (fabs(term) > eps) {
        sum += term;
        n++;
        /* Коэффициент рекуррентности: 27 * n^3 * x^2 / ((3n - 2) * (3n - 1) * 3n) */
        term *= (27.0 * n * n * n * x * x) / ((3.0 * n - 2.0) * (3.0 * n - 1.0) * (3.0 * n));
    }
    *res = sum;
    return 0;
}

//1.d
int sum_d(double x, double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    if (fabs(x) >= 1.0) {
        return 1; /* Ряд сходится только при |x| < 1 */
    }
    double sum = 0.0;
    double term = - (1.0 * x * x) / 2.0; /* При n = 1: (-1)^1 * 1!! * x^2 / 2!! = -x^2 / 2 */
    int n = 1;

    while (fabs(term) > eps) {
        sum += term;
        n++;
        term *= - (2.0 * n - 1.0) * x * x / (2.0 * n);
    }
    *res = sum;
    return 0;
}

//a
int integrate_a(double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    int n = 100;
    double prev_sum = 0.0;
    double curr_sum = 0.0;

    do {
        prev_sum = curr_sum;
        double h = 1.0 / n;
        /* lim_{x->0} ln(1+x)/x = 1 */
        double sum = 0.5 * (1.0 + log(2.0)); 

        for (int i = 1; i < n; i++) {
            double x = i * h;
            sum += log(1.0 + x) / x;
        }
        curr_sum = sum * h;
        n *= 2;
    } while (fabs(curr_sum - prev_sum) > eps && n < 1000000);

    *res = curr_sum;
    return 0;
}

//b
int integrate_b(double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    int n = 100;
    double prev_sum = 0.0;
    double curr_sum = 0.0;

    do {
        prev_sum = curr_sum;
        double h = 1.0 / n;
        double sum = 0.5 * (exp(0.0) + exp(-0.5));

        for (int i = 1; i < n; i++) {
            double x = i * h;
            sum += exp(-x * x / 2.0);
        }
        curr_sum = sum * h;
        n *= 2;
    } while (fabs(curr_sum - prev_sum) > eps && n < 1000000);

    *res = curr_sum;
    return 0;
}

//c
int integrate_c(double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    int n = 100;
    double prev_sum = 0.0;
    double curr_sum = 0.0;

    do {
        prev_sum = curr_sum;
        double h = 1.0 / n;
        double x_start = 0.0;
        double x_end = 1.0 - h / 2.0;
        double sum = 0.5 * (-log(1.0 - x_start) - log(1.0 - x_end));

        for (int i = 1; i < n; i++) {
            double x = i * h;
            sum += -log(1.0 - x);
        }
        curr_sum = sum * h;
        n *= 2;
    } while (fabs(curr_sum - prev_sum) > eps && n < 1000000);

    *res = curr_sum;
    return 0;
}

//d
int integrate_d(double eps, double *res) {
    if (res == NULL || eps <= 0.0) {
        return 1;
    }
    int n = 100;
    double prev_sum = 0.0;
    double curr_sum = 0.0;

    do {
        prev_sum = curr_sum;
        double h = 1.0 / n;
        /* lim_{x->0} x^x = 1 */
        double sum = 0.5 * (1.0 + pow(1.0, 1.0));

        for (int i = 1; i < n; i++) {
            double x = i * h;
            sum += pow(x, x);
        }
        curr_sum = sum * h;
        n *= 2;
    } while (fabs(curr_sum - prev_sum) > eps && n < 1000000);

    *res = curr_sum;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <эпсилон> <номер_задачи_1-8> [значение_x]\n", argv[0]);
        printf("Задачи:\n");
        printf("  1: Сумма a (e^x)\n");
        printf("  2: Сумма b (cos(x))\n");
        printf("  3: Сумма c\n");
        printf("  4: Сумма d\n");
        printf("  5: Интеграл a\n");
        printf("  6: Интеграл b\n");
        printf("  7: Интеграл c\n");
        printf("  8: Интеграл d\n");
        return 1;
    }

    double eps;
    if (parse_double(argv[1], &eps) != 0 || eps <= 0.0 || eps >= 1.0) {
        printf("Ошибка: Некорректное значение эпсилон.\n");
        return 1;
    }

    int task;
    if (parse_double(argv[2], &(double){0}) != 0) {
        /* Парсим номер задачи */
    }
    task = atoi(argv[2]);

    double x = 0.0;
    if (task >= 1 && task <= 4) {
        if (argc < 4) {
            printf("Ошибка: Для вычисления суммы требуется аргумент x.\n");
            return 1;
        }
        if (parse_double(argv[3], &x) != 0) {
            printf("Ошибка: Некорректное значение x.\n");
            return 1;
        }
    }

    double result = 0.0;
    int status = 0;

    switch (task) {
        case 1:
            status = sum_a(x, eps, &result);
            if (status == 0) printf("Сумма a(x=%g) = %.10f\n", x, result);
            break;
        case 2:
            status = sum_b(x, eps, &result);
            if (status == 0) printf("Сумма b(x=%g) = %.10f\n", x, result);
            break;
        case 3:
            status = sum_c(x, eps, &result);
            if (status == 0) printf("Сумма c(x=%g) = %.10f\n", x, result);
            break;
        case 4:
            status = sum_d(x, eps, &result);
            if (status == 0) printf("Сумма d(x=%g) = %.10f\n", x, result);
            break;
        case 5:
            status = integrate_a(eps, &result);
            if (status == 0) printf("Интеграл a = %.10f\n", result);
            break;
        case 6:
            status = integrate_b(eps, &result);
            if (status == 0) printf("Интеграл b = %.10f\n", result);
            break;
        case 7:
            status = integrate_c(eps, &result);
            if (status == 0) printf("Интеграл c = %.10f\n", result);
            break;
        case 8:
            status = integrate_d(eps, &result);
            if (status == 0) printf("Интеграл d = %.10f\n", result);
            break;
        default:
            printf("Ошибка: Неверный номер задачи (должен быть от 1 до 8).\n");
            return 1;
    }

    if (status != 0) {
        printf("Ошибка при вычислении (возможно, недопустимый x или некорректные параметры).\n");
        return 1;
    }

    return 0;
}
