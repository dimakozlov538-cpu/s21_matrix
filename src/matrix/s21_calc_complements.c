#include "s21_matrix.h"
#include <math.h>

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int status = OK;

    if (!A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0 || 
        A->rows != A->columns) {
        status = INCORRECT_MATRIX;
    }

    if (status == OK) {
        if (s21_create_matrix(A->rows, A->columns, result) != OK) {
            status = CALCULATION_ERROR;
        }
    }

    if (status == OK) {
        for (int i = 0; i < A->rows && status == OK; ++i) {
            for (int j = 0; j < A->columns && status == OK; ++j) {
                matrix_t minor = {0};
                
                if (s21_get_minor(A, i, j, &minor) == OK) {
                    double det = 0.0;
                    if (s21_determinant(&minor, &det) == OK) {
                        result->matrix[i][j] = ((i + j) % 2 == 0) ? det : -det;
                    } else {
                        status = CALCULATION_ERROR;
                    }
                    s21_remove_matrix(&minor);
                } else {
                    status = CALCULATION_ERROR;
                }
            }
        }
    }

    if (status != OK && result && result->matrix) {
        s21_remove_matrix(result);
    }
    
    return status;
}