#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//валидация точности из аргументов командной строки
int parse_eps(const char *str, double *eps) {
    if (str == NULL || eps == NULL || str[0] == '\0') {
        return 1;
    }
    char *end = NULL;
    double val = strtod(str, &end); 

    if (*end != '\0' || val <= 0.0 || val >= 1.0) {
        return 1;
    }
    *eps = val;
    return 0;
}

//проверка числа на простоту
int is_prime_num(int n) {
    if (n < 2) return 0;
    for (int i = 2; i<= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

//вычисление сочетаний C
double comb(int n, int k) {
    double res = 1.0;
    for (int i = 1; i <= k; i++) {
        res = res * (n - k + i) / i;
    }
    return res;
}

//e

//e через предел
int e_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double n = 1.0;
    double prev = 0.0;
    double curr = pow(1.0 + 1.0 / n, n);
    
    // увеличиваем n, пока разница между шагами не станет меньше eps
    while (fabs(curr - prev) > eps) {
        prev = curr;
        n *= 2.0;
        curr = pow(1.0 + 1.0 / n, n);
    }
    *res = curr;
    return 0;
}

// 2 e через ряд
int e_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0; // Первый член 1/0! = 1
    int n = 0;

    // Складываем члены ряда, пока очередной член больше eps
    while (term > eps) {
        sum += term;
        n++;
        term = term / n; // Следующий член получается делением предыдущего на n
    }
    *res = sum; 
    return 0;
}

// 3 e через уравнение
int e_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 2.0; // Начальное приближение
    double f = log(x) - 1.0;
    while (fabs(f) > eps) {
        x = x - f * x;
        f = log(x) - 1.0;
    }
    *res = x;
    return 0;
}

//pi

// 1. pi через предел (формула Уоллиса/Предел)
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
    } while (fabs(curr - prev) > eps && n < 100000); // Ограничение шагов для защиты от зацикливания
    
    *res = curr;
    return 0;
}

// 2 pi через ряд
int pi_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) > eps) {
        term = (n % 2 != 0 ? 1.0 : -1.0) / (2.0 * n - 1.0);
        sum += term;
        n++;
    }
    *res = 4.0 * sum;
    return 0;
}

// 3 pi через уравнение
int pi_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 3.0; // Начальное
    double f = cos(x) + 1.0;
    
    while (fabs(f) > eps) {
        x = x - f / (-sin(x));
        f = cos(x) + 1.0;
    }
    *res = x;
    return 0;
}

//ln(2)

// 1 ln(2) через предел: lim_{n -> inf} n * (2^(1/n) - 1)
int ln2_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double n = 1.0;
    double prev = 0.0;
    double curr = n * (pow(2.0, 1.0 / n) - 1.0);
    
    while (fabs(curr - prev) > eps) {
        prev = curr;
        n *= 2.0;
        curr = n * (pow(2.0, 1.0 / n) - 1.0);
    }
    *res = curr;
    return 0;
}

// 2 ln(2) через ряд
int ln2_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    double term = 1.0;
    int n = 1;
    
    while (fabs(term) > eps) {
        term = (n % 2 != 0 ? 1.0 : -1.0) / n;
        sum += term;
        n++;
    }
    *res = sum;
    return 0;
}

// 3 ln(2)через уравнение
int ln2_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 0.5; // Начальное приближение
    double f = exp(x) - 2.0;
    
    while (fabs(f) > eps) {
        x = x - f / exp(x);
        f = exp(x) - 2.0;
    }
    *res = x;
    return 0;
}

//sqrt(2)

// 1 sqrt(2) через предел
int sqrt2_limit(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = -0.5;
    double prev = 0.0;
    
    do {
        prev = x;
        x = x - (x * x) / 2.0 + 1.0;
    } while (fabs(x - prev) > eps);
    
    *res = x;
    return 0;
}

// 2 sqrt(2) через бесконечное произведение
int sqrt2_product(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double prod = 1.0;
    int k = 2;
    double term = pow(2.0, pow(2.0, -k));
    
    // Перемножаем элементы, пока очередной сомножитель отличается от 1 больше чем на eps
    while (fabs(term - 1.0) > eps) {
        prod *= term;
        k++;
        term = pow(2.0, pow(2.0, -k));
    }
    *res = prod;
    return 0;
}

// 3 sqrt(2) через уравнение
int sqrt2_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double x = 1.0; // Начальное приближение
    double f = x * x - 2.0;
    
    while (fabs(f) > eps) {
        x = x - f / (2.0 * x);
        f = x * x - 2.0;
    }
    *res = x;
    return 0;
}

//gamma - постоянная эйлера

// 1. gamma через предел
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
    } while (fabs(curr - prev) > eps && m < 50);
    
    *res = curr;
    return 0;
}

// 2 gamma через ряд
int gamma_series(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double sum = 0.0;
    int k = 2;
    double term = 1.0;
    
    while (fabs(term) > eps) {
        int floor_sqrt = (int)floor(sqrt(k));
        term = 1.0 / (floor_sqrt * floor_sqrt) - 1.0 / (double)k;
        sum += term;
        k++;
    }
    *res = - (M_PI * M_PI) / 6.0 + sum;
    return 0;
}

// 3. gamma через уравнение (предел с произведениями простых чисел)
int gamma_equation(double eps, double *res) {
    if (eps <= 0.0 || res == NULL) return 1;
    double t = 10.0;
    double prev = 0.0;
    double curr = 0.0;
    
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
    } while (fabs(curr - prev) > eps && t < 10000.0);
    
    *res = curr;
    return 0;
}

int main(int argc, char *argv[]) {
    //проверка количества аргументов
    if (argc != 2) {
        printf("Ошибка: передайте точность eps.\n");
        return 1;
    }

    //чтение и валидация epsilon
    double eps = 0.0;
    if (parse_eps(argv[1], &eps) != 0) {
        printf("Ошибка: недопустимое значение eps.\n");
        return 1;
    }

    //переменные - сохранения результатов трех методов
    double val1 = 0.0, val2 = 0.0, val3 = 0.0;

    // Вычисление константы e
    e_limit(eps, &val1);
    e_series(eps, &val2);
    e_equation(eps, &val3);
    printf("e:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // Вычисление константы pi
    pi_limit(eps, &val1);
    pi_series(eps, &val2);
    pi_equation(eps, &val3);
    printf("pi:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // Вычисление константы ln(2)
    ln2_limit(eps, &val1);
    ln2_series(eps, &val2);
    ln2_equation(eps, &val3);
    printf("ln(2):\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // Вычисление константы sqrt(2)
    sqrt2_limit(eps, &val1);
    sqrt2_product(eps, &val2);
    sqrt2_equation(eps, &val3);
    printf("sqrt(2):\n  Предел:       %.10f\n  Произведение: %.10f\n  Уравнение:    %.10f\n\n", val1, val2, val3);

    // Вычисление gamma
    gamma_limit(eps, &val1);
    gamma_series(eps, &val2);
    gamma_equation(eps, &val3);
    printf("gamma:\n  Предел:       %.10f\n  Ряд:          %.10f\n  Уравнение:    %.10f\n", val1, val2, val3);

    return 0;
}
