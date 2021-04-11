#define _CRT_SECURE_NO_WARNINGS

#include "matrix_input.h"
#include <stdio.h>
#include <string.h>




void input_int(int* a) {
    int n;
    do {
        n = scanf("%d", a);
        if (n == 0 || n < 0) {
            printf("You're wrong! Please, try again\n");
        }
    } while (n == 0 || n < 0);
}

void input_double(double* a) {
    int n;
    do {
        n = scanf("%lf", a);
        if (n == 0 || n < 0) {
            printf("You're wrong! Please, try again\n");
        }
    } while (n == 0 || n < 0);
}

void input(Matrix* matr, int lines) {
    int column, j;
    matr->number_lines = lines;
    matr->line = (Line*)calloc(lines, sizeof(Line));//выделяем память в для матрицы (для хранения инфы о кол-ве эл в сстроке и адреса 1 элемента)
    for (int i = 0; i < lines; ++i) {
        printf("Please, enter number of values in line %d:\n", i);
        do {
            input_int(&column);
            if (column <= 0) {
                printf("You're wrong! Please, try again\n");
            }
        } while (column <= 0);
        matr->line[i].j = column;
        matr->line[i].value = (double*)calloc(column, sizeof(double));//выделение памяти под элементы в строке
        printf("Please, enter values in this line:\n");
        for (j = 0; j < column; ++j) {
            input_double(&matr->line[i].value[j]);//ввод элементов в строку
        }  
    }
}

void output(Matrix* matr) {
    for (int i = 0; i < matr->number_lines; ++i) {
        for (int j = 0; j < matr->line[i].j; ++j) {
            printf("%lf", matr->line[i].value[j]);
            printf("\t");
        }
        printf("\n");
    }
}

void new_matrix(Matrix* matr, Matrix* new_matr) {
    int i, j1 = 0, j2 = 0, flagNULL = 0, flagPOS = 0, i_new;
    new_matr->number_lines = matr->number_lines;
    new_matr->line = (Line*)calloc(matr->number_lines, sizeof(Line));
    printf("\nNew matrix:\n");//разделить логику обращения и логику вычисления
    for (i = 0, i_new = 0; i < matr->number_lines; ++i, ++i_new) {
        while (matr->line[i].value[j1] <= 0 && matr->line[i].j > j1) {
            if (matr->line[i].value[j1] == 0) {
                flagNULL++;
            }
            j1++;
        }
        if (j1 == matr->line[i].j && flagNULL != j1) { //случай, если вся строка - неположительна, но и полностью ненулевая
            new_matr->line[i_new].j = matr->line[i].j;
            new_matr->line[i_new].value = (double*)calloc(new_matr->line[i_new].j, sizeof(double));
            for (int k = 0; k < matr->line[i].j; ++k) {
                //new_matr->line[i_new].j++;
                //if (new_matr->line[i_new].j == 1){
               //     new_matr->line[i_new].value = malloc(sizeof (double ));
                //} else{
                 //   new_matr->line[i_new].value = realloc(new_matr->line[i_new].value, new_matr->line[i_new].j*sizeof (double ));
                //}
                new_matr->line[i_new].value[k] = matr->line[i].value[k];
            }
            j1 = 0;
            flagNULL = 0;
            continue;//переходим к следующей итерации
        }
        if (j1 == matr->line[i].j && flagNULL == j1) {//если вся строка нули
            new_matr->number_lines--;
            new_matr->line = (Line*) realloc(new_matr->line, new_matr->number_lines * sizeof(struct Line));
            i_new--;
            j1 = 0;
            flagNULL = 0;
            continue;
        }
        flagPOS++;//т.к. найденный после while элемент точно положительный
        j1++;
        while (matr->line[i].value[j1] >= 0 && matr->line[i].j > j1) {
            flagPOS++;
            new_matr->line[i_new].j++;
            if (new_matr->line[i_new].j == 1) {
                new_matr->line[i_new].value = (double*) malloc(sizeof(double));
            }
            else {
                new_matr->line[i_new].value = (double*)realloc(new_matr->line[i_new].value, new_matr->line[i_new].j * sizeof(double));
            }
            new_matr->line[i_new].value[j2] = matr->line[i].value[j1];
            j1++;
            j2++;
        }
        if (flagPOS == matr->line[i].j) {/*проблема по которой приходиться перезаписывать данную строку - невхождение
        первого положительного элемента строки в строку новой матрицы (т.к. в условии говориться о элементах между 1 пол
        и отрицательным элементом, т.е. не включая первый положительный элемент) данная запись может быть отчасти неэффективна
        для данного случая, но в общем случае она эффективна (т.к. не придется 2 раза прогонять массив)*/
            free(new_matr->line[i_new].value);
            new_matr->line[i_new].j = matr->line[i].j;
            new_matr->line[i_new].value = (double*)calloc(new_matr->line[i_new].j, sizeof(double));
            for (int f = 0; f < new_matr->line[i_new].j; ++f) {
                new_matr->line[i_new].value[f] = matr->line[i].value[f];
            }
        }
        j1 = 0;
        j2 = 0;
        flagNULL = 0;
        flagPOS = 0;
    }
}

void del(Matrix* matr) {
    for (int i = 0; i < matr->number_lines; ++i) {
        free(matr->line[i].value);
    }
    free(matr->line);
    matr->number_lines = 0;
    matr = NULL;
}