#include "s21_matrix.h"
#include <math.h>

static int compute_cofactor_element(matrix_t *A, matrix_t *result, int i, int j) {
    int status = OK;
    matrix_t minor = {0};
    
    if (s21_get_minor(A, i, j, &minor) != OK) {
        status = CALCULATION_ERROR;
    }
    
    if (status == OK) {
        double det = 0.0;
        if (s21_determinant(&minor, &det) != OK) {
            status = CALCULATION_ERROR;
        } else {
            result->matrix[i][j] = ((i + j) % 2 == 0) ? det : -det;
        }
    }
    
    s21_remove_matrix(&minor);
    return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int status = OK;
    
    if (!A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0 || 
        A->rows != A->columns) {
        return INCORRECT_MATRIX;
    }
    
    if (s21_create_matrix(A->rows, A->columns, result) != OK) {
        return CALCULATION_ERROR;
    }
    
    for (int i = 0; i < A->rows && status == OK; ++i) {
        for (int j = 0; j < A->columns && status == OK; ++j) {
            status = compute_cofactor_element(A, result, i, j);
        }
    }
    
    if (status != OK && result && result->matrix) {
        s21_remove_matrix(result);
    }
    
    return status;
}