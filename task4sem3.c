#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef enum{
    STATUS_OK,
    STATUS_ERROR_VALUE,
    STATUS_ERROR_FILE,
    STATUS_ERROR_NULL,
    STATUS_ERROR_MEMORY
}Status;

Status make_out_path(const char *path, char **out){
    if(path == NULL || out == NULL){
        return STATUS_ERROR_NULL;
    }
    size_t size = 0;
    Status st = str_len(path, &size);
    if(st != STATUS_OK){
        return st;
    }
    if(size == 0 || size > SIZE_MAX - 5){
        return STATUS_ERROR_VALUE;
    }
    char *out_path = malloc((size + 5) * sizeof(*out_path));
    if(out_path == NULL){
        return STATUS_ERROR_MEMORY;
    }
    const char *prefix = "out_";
    size_t pos = 0;
    for(size_t i = 0; i < 4; i++){
        out_path[pos] = prefix[pos];
        pos++;
    }
    for(size_t i = 0; i < size; i++){
        out_path[pos] = path[pos-4];
        pos++;
    }
    out_path[pos] = '\0';
    *out = out_path;
    return STATUS_OK;
}
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

// -s
Status only_other_symbols(const char*filename, const char *outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }
    FILE *out = fopen(outname, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }
    int c = 0;
    size_t count = 0;
    int data = 0;
    while((c = fgetc(in)) != EOF){
        if(c == '\n'){
            fprintf(out, "%zu\n", count);
            count = 0;
            data = 0;
        }else{
            data = 1;
            int letter = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
            int digit = (c >= '0' && c <= '9');
            if(!letter && !digit && c != ' '){
                count++;
            }
        }
    }
    if(data){
        fprintf(out, "%zu\n", count); //нету букв и цифр
    }
    fclose(in);
    fclose(out);
    return STATUS_OK;
}
// -i
Status only_letters(const char *filename, const char *outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }
    FILE *out = fopen(outname, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }
    int c;
    size_t count = 0;
    int data = 0;
    while((c = fgetc(in)) != EOF){
        if(c == '\n'){
            fprintf(out, "%zu\n", count);
            data = 0;
            count = 0;
        }
        else{
            data = 1;
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                count++;
            }
        }
    }
    if(data){
        fprintf(out, "%zu\n", count);
    }
    return STATUS_OK;
}

//d
Status no_digits(const char *filename, const char *outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }

    FILE *out = fopen(filename, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }
    int c;
    while((c = fgetc(in)) != EOF){
        if(c == '\n'){
            continue; 
        }
        fputc(c, out);
    }
    fclose(in);
    fclose(out);
    return STATUS_OK;
}

//a
Status no_digits_ascii(const char *filename, const char *outname){
    if(filename == NULL || outname == NULL){
        return STATUS_ERROR_NULL;
    }
    FILE *in = fopen(filename, "r");
    if(in == NULL){
        return STATUS_ERROR_FILE;
    }

    FILE *out = fopen(outname, "w");
    if(out == NULL){
        fclose(in);
        return STATUS_ERROR_FILE;
    }

    const char *hex = "0123456789ABCDEF";
    int c;
    while((c = fgetc(in)) != EOF){
        if(c >= '0' && c <= '9'){
            fputc(c, out);             
        }else{
            fputc(hex[c / 16], out);    // старшая 
            fputc(hex[c % 16], out);    // младшая 
        }
    }
    fclose(in);
    fclose(out);
    return STATUS_OK;
}


void print_error(Status st){
    switch(st){
        case STATUS_ERROR_NULL:
            fprintf(stderr, "Ошибка: передан нулевой указатель\n");
            break;
        case STATUS_ERROR_VALUE:
            fprintf(stderr, "Ошибка: некорректное значение\n");
            break;
        case STATUS_ERROR_FILE:
            fprintf(stderr, "Ошибка: не удалось открыть файл\n");
            break;
        case STATUS_ERROR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память\n");
            break;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            break;
    }
}

int main(int argc, char *argv[]){
    int n = 0;
    char *flag = argv[1];
    if((argc != 3) && (argc != 4)){
        printf("Подано неверное число аргументов!\n");
        return 1;
    }
    char fflag = 0;                                  
    size_t size_flag = 0;
    Status st = str_len(flag, &size_flag);          
    if(st != STATUS_OK){
        printf("Ошибка при разборе флага\n");        
        return 1;
    }
    if(size_flag != 3  && size_flag != 2){
        printf("Неверная длина флага\n");            
        return 1;
    }
    if(size_flag == 2){
        if(flag[0] != '/' && flag[0] != '-'){        
            printf("Флаг должен начинаться с - или /\n");
            return 1;
        }
        fflag = flag[1];                            
        if(fflag != 's' && fflag != 'd' && fflag != 'i' && fflag != 'a'){  
            printf("Подан неверный флаг\n");
            return 1;
        }
        n = 0;
    }
    if(size_flag == 3){
        if((flag[0] != '/' && flag[0] != '-') || flag[1] != 'n'){  
            printf("Флаг должен быть вида -n? или /n?\n");
            return 1;
        }
        fflag = flag[2];                             
        if(fflag != 'i' && fflag != 'a' && fflag != 's' && fflag != 'd'){  
            printf("Неверно подан флаг!\n");
            return 1;                                
        }
        n = 1;
    }

    
    if((n == 1 && argc != 4) || (n == 0 && argc != 3)){
        printf("Неверное число аргументов для этого флага\n");
        return 1;
    }

    // ДОБАВЛЕНО: путь к выходному файлу
    const char *in_path = argv[2];
    char *gen = NULL;
    const char *out_path = NULL;
    if(n == 1){
        out_path = argv[3];
    }else{
        st = make_out_path(in_path, &gen);
        if(st != STATUS_OK){
            printf("Не удалось сформировать имя выходного файла\n");
            return 1;
        }
        out_path = gen;
    }

    switch(fflag){
        case 'd':
            st = no_digits(in_path, out_path);
            break;
        case 'i':
            st = only_letters(in_path, out_path);
            break;
        case 's':
            st = only_other_symbols(in_path, out_path);
            break;
        default:                                     // 'a'
            st = no_digits_ascii(in_path, out_path);
            break;
    }

    free(gen);                                
    if(st != STATUS_OK){
        printf("Ошибка при обработке файла\n");
        return 1;
    }
    return 0;
}



