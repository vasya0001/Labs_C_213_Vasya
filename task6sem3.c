#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define EPS 1e-9

typedef struct {
    double x;
    double y;
} Point;


// 6.1. Выпуклость многоугольника

int is_convex_polygon(int *is_convex, int count, ...) {
    if (is_convex == NULL || count < 3) {
        return 1;
    }

    Point *pts = (Point *)malloc(count * sizeof(Point));
    if (pts == NULL) {
        return 2; // Ошибка выделения памяти
    }

    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        pts[i].x = va_arg(args, double);
        pts[i].y = va_arg(args, double);
        if (isnan(pts[i].x) || isinf(pts[i].x) || isnan(pts[i].y) || isinf(pts[i].y)) {
            va_end(args);
            free(pts);
            return 1;
        }
    }
    va_end(args);

    int sign = 0;
    *is_convex = 1;

    for (int i = 0; i < count; i++) {
        Point p1 = pts[i];
        Point p2 = pts[(i + 1) % count];
        Point p3 = pts[(i + 2) % count];

        // Косое произведение векторов (p1->p2) и (p2->p3)
        double cross = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

        if (fabs(cross) > EPS) {
            int current_sign = (cross > 0.0) ? 1 : -1;
            if (sign == 0) {
                sign = current_sign;
            } else if (sign != current_sign) {
                *is_convex = 0;
                break;
            }
        }
    }

    free(pts);
    return 0;
}

// 
// 6.2. Вычисление многочлена по схеме Горнера
// 
int eval_polynomial(double *res, double x, int n, ...) {
    if (res == NULL || n < 0 || isnan(x) || isinf(x)) {
        return 1;
    }

    va_list args;
    va_start(args, n);

    // Старший коэффициент (при x^n)
    double result = va_arg(args, double);
    if (isnan(result) || isinf(result)) {
        va_end(args);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        double coef = va_arg(args, double);
        if (isnan(coef) || isinf(coef)) {
            va_end(args);
            return 1;
        }
        result = result * x + coef;
        if (isnan(result) || isinf(result)) {
            va_end(args);
            return 2; // Переполнение типа double
        }
    }

    va_end(args);

    *res = result;
    return 0;
}


// Вспомогательные функции для 6.3 (Числа Капрекара)

static int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

static int string_to_ull(const char *str, int base, unsigned long long *val) {
    if (str == NULL || val == NULL || base < 2 || base > 36 || str[0] == '\0') {
        return 1;
    }
    unsigned long long res = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        int d = char_to_digit(str[i]);
        if (d < 0 || d >= base) return 1;

        if (res > (ULLONG_MAX - d) / base) {
            return 2; // Переполнение
        }
        res = res * base + d;
    }
    *val = res;
    return 0;
}

static int check_kaprekar(const char *str, int base, int *is_kap) {
    if (str == NULL || is_kap == NULL) return 1;
    *is_kap = 0;

    unsigned long long n = 0;
    if (string_to_ull(str, base, &n) != 0) return 1;

    if (n == 1) {
        *is_kap = 1;
        return 0;
    }

    if (n > 0 && n > ULLONG_MAX / n) {
        return 2; // Переполнение при квадрировании
    }

    unsigned long long square = n * n;
    unsigned long long power = 1;

    while (power <= square / base) {
        power *= base;
        unsigned long long high = square / power;
        unsigned long long low = square % power;

        if (low > 0 && high + low == n) {
            *is_kap = 1;
            return 0;
        }
    }

    return 0;
}

// 6.3. Поиск чисел Капрекара (вариативная функция)

int find_kaprekar_numbers(int base, int count, ...) {
    if (base < 2 || base > 36 || count <= 0) {
        return 1;
    }

    va_list args;
    va_start(args, count);

    printf("Числа Капрекара в СС с основанием %d:\n", base);
    int found = 0;

    for (int i = 0; i < count; i++) {
        const char *num_str = va_arg(args, const char *);
        int is_kap = 0;
        if (check_kaprekar(num_str, base, &is_kap) == 0 && is_kap) {
            printf("  -> %s\n", num_str);
            found++;
        }
    }

    va_end(args);

    if (found == 0) {
        printf("  (не найдено)\n");
    }

    return 0;
}

// 6.4. Среднее геометрическое (последний обязательный параметр — count)
int geometric_mean(double *res, int count, ...) {
    if (res == NULL || count <= 0) {
        return 1;
    }

    va_list args;
    va_start(args, count);

    double sum_log = 0.0;
    for (int i = 0; i < count; i++) {
        double val = va_arg(args, double);
        if (val <= 0.0 || isnan(val) || isinf(val)) {
            va_end(args);
            return 1; // Среднее геометрическое строго для положительных чисел
        }
        sum_log += log(val);
    }

    va_end(args);

    *res = exp(sum_log / count);
    return 0;
}

// 6.5. Рекурсивное быстрое возведение в степень (O(log n))
int fast_pow_rec(double base, int exp, double *res) {
    if (res == NULL || isnan(base) || isinf(base)) {
        return 1;
    }

    if (exp == 0) {
        *res = 1.0;
        return 0;
    }

    if (exp < 0) {
        if (exp == INT_MIN) return 1; // Защита от переполнения знака при -INT_MIN
        double temp = 0.0;
        if (fast_pow_rec(base, -exp, &temp) != 0 || fabs(temp) < EPS) {
            return 1; // Деление на ноль
        }
        *res = 1.0 / temp;
        return 0;
    }

    double half = 0.0;
    if (fast_pow_rec(base, exp / 2, &half) != 0) {
        return 1;
    }

    double sq = half * half;
    if (isnan(sq) || isinf(sq)) return 2;

    if (exp % 2 == 0) {
        *res = sq;
    } else {
        double ans = sq * base;
        if (isnan(ans) || isinf(ans)) return 2;
        *res = ans;
    }

    return 0;
}

// 6.6. Поиск корня методом дихотомии
int find_root_dichotomy(double a, double b, double eps, double (*f)(double), double *root) {
    if (f == NULL || root == NULL || eps <= 0.0 || a >= b) {
        return 1;
    }

    double fa = f(a);
    double fb = f(b);

    if (isnan(fa) || isinf(fa) || isnan(fb) || isinf(fb)) {
        return 1;
    }

    // Проверяем, что функции на концах отрезка имеют разные знаки
    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0)) {
        return 1;
    }

    double mid = a;
    int max_iter = 1000;
    int iter = 0;

    while ((b - a) / 2.0 > eps && iter < max_iter) {
        mid = a + (b - a) / 2.0;
        double fmid = f(mid);

        if (isnan(fmid) || isinf(fmid)) {
            return 1;
        }

        if (fabs(fmid) < EPS) {
            break;
        }

        if ((fa < 0.0 && fmid > 0.0) || (fa > 0.0 && fmid < 0.0)) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
        iter++;
    }

    *root = a + (b - a) / 2.0;
    return 0;
}

// Вспомогательные уравнения для демонстрации 6.6
double f1(double x) { return x * x - 2.0; } // корень sqrt(2) ≈ 1.414213
double f2(double x) { return sin(x); }     // корень pi ≈ 3.141592 на [3, 4]


int main(void) {
    printf("=== DEMO 6.1: Выпуклость многоугольника ===\n");
    int is_conv = 0;
    if (is_convex_polygon(&is_conv, 4, 0.0, 0.0, 2.0, 0.0, 2.0, 2.0, 0.0, 2.0) == 0) {
        printf("Квадрат выпуклый? %s\n", is_conv ? "Да" : "Нет");
    }

    printf("\n=== DEMO 6.2: Схема Горнера ===\n");
    // P(x) = 2x^3 - 3x + 5 при x = 2.0 -> 2*(8) - 6 + 5 = 15
    double poly_res = 0.0;
    if (eval_polynomial(&poly_res, 2.0, 3, 2.0, 0.0, -3.0, 5.0) == 0) {
        printf("P(2.0) = %.2f (Ожидается: 15.00)\n", poly_res);
    }

    printf("\n=== DEMO 6.3: Числа Капрекара ===\n");
    find_kaprekar_numbers(10, 3, "45", "297", "10");

    printf("\n=== DEMO 6.4: Среднее геометрическое ===\n");
    double geom_res = 0.0;
    if (geometric_mean(&geom_res, 3, 2.0, 8.0, 4.0) == 0) {
        printf("Среднее геометрическое (2, 8, 4) = %.4f\n", geom_res);
    }

    printf("\n=== DEMO 6.5: Быстрое возведение в степень ===\n");
    double pow_res = 0.0;
    if (fast_pow_rec(2.0, -3, &pow_res) == 0) {
        printf("2.0^(-3) = %.4f\n", pow_res);
    }

    printf("\n=== DEMO 6.6: Метод дихотомии ===\n");
    double root = 0.0;
    if (find_root_dichotomy(1.0, 2.0, 1e-6, f1, &root) == 0) {
        printf("Корень x^2 - 2 = 0 на [1, 2]: %.6f\n", root);
    }
    if (find_root_dichotomy(3.0, 4.0, 1e-6, f2, &root) == 0) {
        printf("Корень sin(x) = 0 на [3, 4]: %.6f\n", root);
    }

    return 0;
}