
#include <stdlib.h>

typedef struct Line {
    int j;
    double* value;
}Line;

typedef struct Matr {
    int number_lines;
    Line* line;
}Matrix;

void input_int(int*);
void input_double(double*);
void input(Matrix*, int);
void output(Matrix*);
void del(Matrix*);
void new_matrix(Matrix*, Matrix*);