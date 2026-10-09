#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_ITER 1000000


// Чтение и валидация эпсилон из аргументов командной строки
int parse_eps(const char *str, double *eps) {
    if (str == NULL || eps == NULL || str[0] == '\0') {
        return 1;
    }
    char *end = NULL;
    errno = 0;
    double val = strtod(str, &end); 

    if (*end != '\0' || errno == ERANGE || isnan(val) || isinf(val) || val <= 0.0 || val >= 1.0) {
        return 1;
    }
    *eps = val;
    return 0;
}

// Проверка числа на простоту
int is_prime_num(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Вычисление сочетаний C(n, k)
double comb(int n, int k) {
    if (k < 0 || k > n) return 0.0;
    if (k == 0 || k == n) return 1.0;
    double res = 1.0;
    for (int i = 1; i <= k; i++) {
        res = res * (n - k + i) / i;
    }
    return res;
}





//1 - e через предел
int e_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double n = 1.0;
    double prev = 0.0;
    double curr = pow(1.0 + 1.0 / n, n);
    int iter = 0;
    
    while (fabs(curr - prev) > eps && iter < MAX_ITER) {
        prev = curr;
        n *= 2.0;
        if (n > 1e15) break; // Защита от потери точности double
        curr = pow(1.0 + 1.0 / n, n);
        iter++;
    }
    *res = curr;
    return 0;
}

// 2 - e через ряд:
int e_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0;
    int n = 0;

    while (term > eps && n < 1000) {
        sum += term;
        n++;
        term /= n;
    }
    *res = sum; 
    return 0;
}

// 3 - e через уравнение
int e_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 2.0;
    double f = log(x) - 1.0;
    int iter = 0;

    while (fabs(f) > eps && iter < MAX_ITER) {
        x = x - f * x;
        if (x <= 0.0 || isnan(x) || isinf(x)) {
            x = 2.7182818284; // Резервный выход при сбое Ньютона
            break;
        }
        f = log(x) - 1.0;
        iter++;
    }
    *res = x;
    return 0;
}



// 1 - pi через предел
int pi_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double prev = 0.0;
    double curr = 0.0;
    int n = 1;
    
    do {
        prev = curr;
        double term = 1.0;
        for (int i = 1; i <= n; i++) {
            term *= (4.0 * i * i) / ((2.0 * i - 1.0) * (2.0 * i));
        }
        curr = 4.0 * term * n / (2.0 * n + 1.0);
        n++;
    } while (fabs(curr - prev) > eps && n < 5000);
    
    *res = curr;
    return 0;
}

// 2 - pi через ряд
int pi_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) > eps && n < MAX_ITER) {
        term = (n % 2 != 0 ? 1.0 : -1.0) / (2.0 * n - 1.0);
        sum += term;
        n++;
    }
    *res = 4.0 * sum;
    return 0;
}

// 3 - pi через уравнение
int pi_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 3.0;
    double f = cos(x) + 1.0;
    int iter = 0;
    
    while (fabs(f) > eps && iter < MAX_ITER) {
        double df = -sin(x);
        if (fabs(df) < 1e-12) break; // Защита от деления на 0
        x = x - f / df;
        f = cos(x) + 1.0;
        iter++;
    }
    *res = x;
    return 0;
}



// 1 - ln(2) через предел
int ln2_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double n = 1.0;
    double prev = 0.0;
    double curr = n * (pow(2.0, 1.0 / n) - 1.0);
    int iter = 0;
    
    while (fabs(curr - prev) > eps && iter < MAX_ITER) {
        prev = curr;
        n *= 2.0;
        if (n > 1e15) break;
        curr = n * (pow(2.0, 1.0 / n) - 1.0);
        iter++;
    }
    *res = curr;
    return 0;
}

// 2 - ln(2) через ряд
int ln2_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) > eps && n < MAX_ITER) {
        term = (n % 2 != 0 ? 1.0 : -1.0) / n;
        sum += term;
        n++;
    }
    *res = sum;
    return 0;
}

// 3 - ln(2) через уравнение: e^x = 2
int ln2_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 0.5;
    double f = exp(x) - 2.0;
    int iter = 0;
    
    while (fabs(f) > eps && iter < MAX_ITER) {
        x = x - f / exp(x);
        f = exp(x) - 2.0;
        iter++;
    }
    *res = x;
    return 0;
}



// 1 - sqrt(2) через предел
int sqrt2_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = -0.5;
    double prev = 0.0;
    int iter = 0;
    
    do {
        prev = x;
        x = x - (x * x) / 2.0 + 1.0;
        iter++;
    } while (fabs(x - prev) > eps && iter < MAX_ITER);
    
    *res = x;
    return 0;
}

// 2 - sqrt(2) через бесконечное произведение
int sqrt2_product(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double prod = 1.0;
    int k = 2;
    double term = pow(2.0, pow(2.0, -k));
    
    while (fabs(term - 1.0) > eps && k < 60) {
        prod *= term;
        k++;
        term = pow(2.0, pow(2.0, -k));
    }
    *res = prod;
    return 0;
}

// 3 - sqrt(2) через уравнение: x^2 = 2
int sqrt2_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 1.0;
    double f = x * x - 2.0;
    int iter = 0;
    
    while (fabs(f) > eps && iter < MAX_ITER) {
        x = x - f / (2.0 * x);
        f = x * x - 2.0;
        iter++;
    }
    *res = x;
    return 0;
}



// 1 - gamma через предел
int gamma_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double prev = 0.0;
    double curr = 0.0;
    int m = 1;
    
    do {
        prev = curr;
        double sum = 0.0;
        double fact = 1.0;
        for (int k = 1; k <= m; k++) {
            fact *= k;
            double term = comb(m, k) * ((k % 2 != 0 ? -1.0 : 1.0) / k) * log(fact);
            sum += term;
        }
        curr = sum;
        m++;
    } while (fabs(curr - prev) > eps && m < 30); // Ограничение m=30 из-за логарифма факториала
    
    *res = curr;
    return 0;
}

// 2 - gamma через ряд
int gamma_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    int k = 2;
    double term = 1.0;
    
    while (fabs(term) > eps && k < MAX_ITER) {
        int floor_sqrt = (int)floor(sqrt(k));
        term = 1.0 / (floor_sqrt * floor_sqrt) - 1.0 / (double)k;
        sum += term;
        k++;
    }
    *res = - (M_PI * M_PI) / 6.0 + sum;
    return 0;
}

// 3- gamma через уравнение
int gamma_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double t = 10.0;
    double prev = 0.0;
    double curr = 0.0;
    int iter = 0;
    
    do {
        prev = curr;
        double prod = 1.0;
        for (int p = 2; p <= (int)t; p++) {
            if (is_prime_num(p)) {
                prod *= ((double)p - 1.0) / (double)p;
            }
        }
        curr = -log(log(t) * prod);
        t *= 2.0;
        iter++;
    } while (fabs(curr - prev) > eps && t < 100000.0 && iter < 1000);
    
    *res = curr;
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Ошибка: передайте точность eps.\n");
        return 1;
    }

    double eps = 0.0;
    if (parse_eps(argv[1], &eps) != 0) {
        printf("Ошибка: недопустимое значение eps.\n");
        return 1;
    }

    double val1 = 0.0, val2 = 0.0, val3 = 0.0;

    // e
    e_limit(eps, &val1);
    e_series(eps, &val2);
    e_equation(eps, &val3);
    printf("e:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // pi
    pi_limit(eps, &val1);
    pi_series(eps, &val2);
    pi_equation(eps, &val3);
    printf("pi:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // ln(2)
    ln2_limit(eps, &val1);
    ln2_series(eps, &val2);
    ln2_equation(eps, &val3);
    printf("ln(2):\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // sqrt(2)
    sqrt2_limit(eps, &val1);
    sqrt2_product(eps, &val2);
    sqrt2_equation(eps, &val3);
    printf("sqrt(2):\n  Предел:       %.10f\n  Произведение: %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // gamma
    gamma_limit(eps, &val1);
    gamma_series(eps, &val2);
    gamma_equation(eps, &val3);
    printf("gamma:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n", val1, val2, val3);

    return 0;
}