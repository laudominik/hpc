


#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <omp_samples.h>

#define MAT_IX(row, col, cols) col + row * cols

typedef struct {
    int rows;
    int cols;
    int* data;
} matrix_t;

matrix_t amatmul(matrix_t mat_a, matrix_t mat_b) {
    matrix_t mat_out;

    assert(mat_a.cols == mat_b.rows);
    mat_out.rows = mat_a.rows;
    mat_out.cols = mat_b.cols;
    mat_out.data = calloc(mat_out.rows * mat_out.cols, sizeof(int));

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < mat_out.rows * mat_out.cols; i++){
        int row = i / mat_out.cols;
        int col = i % mat_out.cols;
        int sum = 0;
        
        #pragma omp parallel for reduction(+:sum) schedule(dynamic)
        for (int j = 0; j < mat_a.cols; j++){
            sum += 
                mat_a.data[MAT_IX(row, j, mat_a.cols)] * 
                mat_b.data[MAT_IX(j, col, mat_b.cols)];
        }
        mat_out.data[i] = sum;
    }

    return mat_out;
}

void matrix_fmt(matrix_t mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++){
            printf("%d\t", mat.data[MAT_IX(i, j, mat.cols)]);
        }
        printf("\n");
    }
    printf("\n");
}


static void wrapper(int _) {
    // columnwise sum
    int A[] = {
        3, 2, 1, 5, 6,
        1, 1, 1, 1, 1,
        2, 2, 2, 2, 2
    };
    int B[] = {
        2, 3,
        8, 8,
        1, 2,
        3, 11,
        7, 8,
    };
    matrix_t mat_a = { .rows = 3, .cols = 5, .data = A };
    matrix_t mat_b = { .rows = 5, .cols = 2, .data = B };
    matrix_t mat_out;

    omp_set_num_threads(8);
    mat_out = amatmul(mat_a, mat_b);
    matrix_fmt(mat_out);
}

SAMPLE(wrapper, 0)
