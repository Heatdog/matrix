#define _CRT_SECURE_NO_WARNINGS
/*Из входного потока вводится непрямоугольная матрица целых чисел [aij], i = 1, …, m, j = 1, …, n.
  Значения m и ni заранее не известны и вводятся из входного потока.

Сформировать новую матрицу, поместив в ее i-ую строку элементы из i-ой строки исходной матрицы,
 размещенные между первым элементом > 0 и первым элементом < 0 (порядок расположения элементов произволен).
 Если в строке нет элемента, удовлетворяющего какому-либо одному условию, то выбираются элементы до конца строки исходной матрицы.
 Если строка исходной матрицы содержит только нули (т.е. не выполняются оба условия),
 то в результирующую матрицу новая строка не включается.

Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями.*/
#include <stdio.h>
#include "matrix_input.h"




int main() {
    int lines;
    printf("Hello, this program creat matrix.\nPlease, enter number of lines: \n");
    do {
        input_int(&lines);
        if (lines <= 0) {
            printf("You're wrong! Please, try again\n");
        }
    } while (lines <= 0);
    Matrix matr = { 0, NULL };
    Matrix new_matr = { 0, NULL };
    input(&matr, lines);
    output(&matr);
    new_matrix(&matr, &new_matr);
    output(&new_matr);
    del(&new_matr);
    del(&matr);
    return 0;
}
