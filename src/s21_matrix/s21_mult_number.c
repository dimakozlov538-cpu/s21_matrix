#include "s21_matrix.h"
#include <stdlib.h>

static void compute_matrix_element(matrix_t *A, matrix_t *B, matrix_t *result, int i, int j, int columns) {
    result->matrix[i][j] = 0.0;
    for (int k = 0; k < columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
    }
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    if (A == NULL || B == NULL || result == NULL) {
        return INCORRECT_MATRIX;
    }
    
    if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
        return INCORRECT_MATRIX;
    }
    
    if (A->columns != B->rows) {
        return CALCULATION_ERROR;
    }
    
    int code = s21_create_matrix(A->rows, B->columns, result);
    if (code != OK) {
        return code;
    }
    
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
            compute_matrix_element(A, B, result, i, j, A->columns);
        }
    }
    
    return OK;
}