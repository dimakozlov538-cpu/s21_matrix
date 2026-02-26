#include "s21_matrix.h"
#include <stdlib.h>

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int code = OK;

    if (A == NULL || B == NULL || result == NULL) {
        code = INCORRECT_MATRIX;
    } else if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0) {
        code = INCORRECT_MATRIX;
    } else if (A->columns != B->rows) {
        code = CALCULATION_ERROR;
    } else {
        code = s21_create_matrix(A->rows, B->columns, result);
        if (code == OK) {
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < B->columns; j++) {
                    result->matrix[i][j] = 0.0;
                    for (int k = 0; k < A->columns; k++) {
                        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                    }
                }
            }
        }
    }

    return code;
}