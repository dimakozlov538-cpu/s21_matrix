#include "s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
    int code = OK;

    if( A == NULL ||result == NULL) {
        code = INCORRECT_MATRIX;
    } else if(A->rows <= 0 || A->columns <= 0) {
        code = INCORRECT_MATRIX;
    } else {
        code = s21_create_matrix(A->columns, A->rows, result);
        if(code == OK) {
            for(int i = 0; i < A->rows; i++) {
                for(int j = 0; j < A->columns; j++) {
                    result->matrix[j][i] = A->matrix[i][j];
                }
            }
        }
    }
    return code;
}