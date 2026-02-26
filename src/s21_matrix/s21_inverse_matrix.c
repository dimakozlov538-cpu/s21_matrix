#include "s21_matrix.h"
#include <math.h>

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
    int status = OK;
    
    if (A == NULL || result == NULL || A->matrix == NULL || A->rows != A->columns) {
        return INCORRECT_MATRIX;
    }
    
    double determinant = 0.0;
    s21_determinant(A, &determinant);
    if (fabs(determinant) < 1e-7) {
        return CALCULATION_ERROR;
    }
    
    matrix_t cofactors = {0};
    matrix_t transposed = {0};
    
    if (s21_calc_complements(A, &cofactors) != OK) {
        status = INCORRECT_MATRIX;
    }
    
    if (status == OK && s21_transpose(&cofactors, &transposed) != OK) {
        status = INCORRECT_MATRIX;
    }
    
    if (status == OK && s21_mult_number(&transposed, 1.0 / determinant, result) != OK) {
        status = INCORRECT_MATRIX;
    }
    
    s21_remove_matrix(&cofactors);
    s21_remove_matrix(&transposed);
    
    return status;
}