#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#define SALARY_EPS 1e-9
typedef enum{
    STATUS_OK,
    STATUS_ERROR_MEMORY,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_FILE
}Status;

typedef struct {
    unsigned int id;
    char *name;
    char *second_name;
    double salary;
} Employee;

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

void free_employees(Employee *arr, size_t count) {
    if (arr == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(arr[i].name);
        free(arr[i].second_name);
    }
    free(arr);
}

Status parse_employee(const char *line, Employee *result){
    if (line == NULL || result == NULL) {
    return STATUS_ERROR_NULL;
    }
    unsigned int result_first = 0;
    size_t len = 0;
    int number_of_arg = 0; //статус, какой щас элемент обрабатывается

    //скип начала
    while(line[len] == ' ' || line[len] == '\t'){
        len++;
    }

    //обраюотка первого элемента
    if(line[len] >= '0' && line[len] <= '9'){
        number_of_arg = 1;
    }else{
        return STATUS_ERROR_VALUE; // первым аргументом был передано не целое число
    }
    while (line[len] >= '0' && line[len] <= '9') {
    unsigned int digit = (unsigned int)(line[len] - '0');
    if (result_first > (UINT_MAX - digit) / 10) {
        return STATUS_ERROR_VALUE;   // переполнение
    }
    result_first = result_first * 10 + digit; //сбор первого аргумента
    len++;
    }
    if (line[len] != ' ' && line[len] != '\t') {
    return STATUS_ERROR_VALUE;
    }
    //скип между 1 и 2
    while(line[len] == ' ' || line[len] == '\t'){
        len++; 
    }

    //обработка второго элемента
    size_t start = len;                      // запомнили начало слова
    size_t count_second = 0;
    while ((line[len] >= 'a' && line[len] <= 'z') ||
        (line[len] >= 'A' && line[len] <= 'Z')) {
        count_second++;
        len++;
    }

    if (count_second == 0 || (line[len] != ' ' && line[len] != '\t')) {
        return STATUS_ERROR_VALUE;           // пусто или после имени не разделитель
    }

    char *second_arg = malloc((count_second + 1) * sizeof(*second_arg));
    if (second_arg == NULL) {
        free(second_arg);
        return STATUS_ERROR_MEMORY;
    }

    for (size_t pos2 = 0; pos2 < count_second; pos2++) {   // второй проход
        second_arg[pos2] = line[start + pos2];
    }
    second_arg[count_second] = '\0';

    //скип между 2 и 3
    while(line[len] == ' ' || line[len] == '\t'){
        len++; 
    }

    //обработка третьего аргумента
    size_t start3 = len;                      // запомнили начало слова
    size_t count_third = 0;
    while ((line[len] >= 'a' && line[len] <= 'z') ||
        (line[len] >= 'A' && line[len] <= 'Z')) {
        count_third++;
        len++;
    }

    if (count_third == 0 || (line[len] != ' ' && line[len] != '\t')) {
        free(second_arg);
        return STATUS_ERROR_VALUE;           // пусто или после имени не разделитель
    }

    char *third_arg = malloc((count_third + 1) * sizeof(*third_arg));
    if (third_arg == NULL) {
        free(second_arg);
        return STATUS_ERROR_MEMORY;
    }

    for (size_t pos3 = 0; pos3 < count_third; pos3++) {   // второй проход
        third_arg[pos3] = line[start3 + pos3];
    }
    third_arg[count_third] = '\0';

    //скип между 3 и 4
    while(line[len] == ' ' || line[len] == '\t'){
        len++; 
    }

    //обработка 4 аргумента
    if(line[len] >= '0' && line[len] <= '9'){
        number_of_arg = 4;
    }else{
        free(second_arg);
        free(third_arg);
        return STATUS_ERROR_VALUE; // первым аргументом был передано не число
    }
    double salary = 0.0;
    // целая часть
    while(line[len] >= '0' && line[len] <= '9'){
        salary = salary * 10 + (double)(line[len] - '0');
        if(salary > 1e300){          //переполнения double
            free(second_arg);
            free(third_arg);
            return STATUS_ERROR_VALUE;
        }
        len++;
    }

    // дробная часть
    if(line[len] == '.'){
        len++;
        if(!(line[len] >= '0' && line[len] <= '9')){
            free(second_arg);
            free(third_arg);
            return STATUS_ERROR_VALUE; // после точки нет цифр
        }
        double scale = 0.1;
        while(line[len] >= '0' && line[len] <= '9'){
            salary += (double)(line[len] - '0') * scale;
            scale /= 10;
            len++;
        }
    }
    //пропускаем пробелы после 4 
    while(line[len] == ' ' || line[len] == '\t'){
        len++;
    }
    if(line[len] != '\n' && line[len] != '\r' && line[len] != '\0'){
        free(second_arg);
        free(third_arg);
        return STATUS_ERROR_VALUE; // мусор после зарплаты или лишнее поле
    }
    result->id = result_first;
    result->name = second_arg;
    result->second_name = third_arg;
    result->salary = salary;
    return STATUS_OK;
}




Status read_employees(const char *path, Employee **arr, size_t *count) {
    if (path == NULL || arr == NULL || count == NULL) {
        return STATUS_ERROR_NULL;
    }
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return STATUS_ERROR_FILE;
    }
    Employee *items = NULL;
    size_t n = 0;
    size_t cap = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        // строка не поместилась в буфер
        size_t i = 0;
        while (line[i] != '\n' && line[i] != '\0') {
            i++;
        }
        if (line[i] == '\0' && !feof(file)) {
            free_employees(items, n);
            fclose(file);
            return STATUS_ERROR_VALUE;
        }

        // пропуск пустых строк
        size_t k = 0;
        while (line[k] == ' ' || line[k] == '\t' || line[k] == '\r') {
            k++;
        }
        if (line[k] == '\n' || line[k] == '\0') {
            continue;
        }

        // разбор строки
        Employee emp;
        Status st = parse_employee(line, &emp);
        if (st != STATUS_OK) {
            free_employees(items, n);
            fclose(file);
            return st;
        }

        // увеличение массива когда надо
        if (n == cap) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            Employee *tmp = realloc(items, new_cap * sizeof(*tmp));
            if (tmp == NULL) {
                free(emp.name);
                free(emp.second_name);
                free_employees(items, n);
                fclose(file);
                return STATUS_ERROR_MEMORY;
            }
            items = tmp;
            cap = new_cap;
        }
        items[n] = emp;
        n++;
    }

    fclose(file);
    *arr = items;
    *count = n;
    return STATUS_OK;
}

int compare_double(double a, double b, double eps) {
    if (a - b > eps) return 1;
    if (b - a > eps) return -1;
    return 0;
}

int compare_str(const char *a, const char *b) {
    size_t i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

int compare_asc(const void *left, const void *right) {
    const Employee *a = left;
    const Employee *b = right;

    int res = compare_double(a->salary, b->salary, SALARY_EPS);
    if (res == 0) res = compare_str(a->second_name, b->second_name);
    if (res == 0) res = compare_str(a->name, b->name);
    if (res == 0) res = (a->id > b->id) - (a->id < b->id);
    return res;
}

int compare_desc(const void *left, const void *right) {
    return compare_asc(right, left);
}

Status write_employees(const char *path, const Employee *arr, size_t count){
    if(path == NULL || (arr == NULL && count > 0)){
        return STATUS_ERROR_NULL;
    }
    FILE *file = fopen(path, "w");
    if(file == NULL){
        return STATUS_ERROR_FILE;
    }
    for(size_t i = 0; i < count; i++){
        if(fprintf(file, "%u %s %s %.2f\n", arr[i].id, arr[i].name,
                   arr[i].second_name, arr[i].salary) < 0){
            fclose(file);
            return STATUS_ERROR_FILE;
        }
    }
    if(fclose(file) != 0){
        return STATUS_ERROR_FILE;
    }
    return STATUS_OK;
}
void print_error(Status st){
    switch(st){
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        case STATUS_ERROR_VALUE:
            fprintf(stderr, "Ошибка: некорректные данные во входном файле\n");
            break;
        case STATUS_ERROR_NULL:
            fprintf(stderr, "Ошибка: передан нулевой указатель\n");
            break;
        case STATUS_ERROR_FILE:
            fprintf(stderr, "Ошибка: не удалось открыть или записать файл\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}
int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Передано некорректное количество параметров!\n");
        return 1;
    }
    char *flag = argv[2];
    size_t len_flag = 0;
    Status st = str_len(flag, &len_flag);
    if(st != STATUS_OK){
        print_error(st);
        return 1;
    }
    if(len_flag != 2  || (flag[0] != '-' && flag[0] != '/')  || (flag[1] != 'a' && flag[1] != 'd')){
        fprintf(stderr, "Неправильно подан сам флаг! - или /, потом a или d\n");
        return 1;
    }

    Employee *arr = NULL;
    size_t count = 0;
    st = read_employees(argv[1], &arr, &count);
    if(st != STATUS_OK){
        print_error(st);
        return 1;
    }

    if(count > 0){
        int (*cmp)(const void *, const void *) = (flag[1] == 'a') ? compare_asc : compare_desc;
        qsort(arr, count, sizeof(*arr), cmp);
    }

    st = write_employees(argv[3], arr, count);
    free_employees(arr, count);   // память освобождается и при успехе, и при ошибке записи
    if(st != STATUS_OK){
        print_error(st);
        return 1;
    }
    return 0;
}
