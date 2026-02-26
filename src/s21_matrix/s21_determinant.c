#include "s21_matrix.h"
#include <math.h>

int s21_determinant(matrix_t *A, double *result) {
    if (!A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0 || 
        A->rows != A->columns) {
        return INCORRECT_MATRIX;
    }

    int status = OK;
    
    if (A->rows == 1) {
        *result = A->matrix[0][0];
    } else if (A->rows == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] - 
                  A->matrix[0][1] * A->matrix[1][0];
    } else {
        *result = 0.0;
        for (int j = 0; j < A->columns && status == OK; ++j) {
            matrix_t minor = {0};
            if (s21_get_minor(A, 0, j, &minor) == OK) {
                double minor_det = 0.0;
                status = s21_determinant(&minor, &minor_det);
                s21_remove_matrix(&minor);

                if (status == OK) {
                    double sign = (j % 2 == 0) ? 1.0 : -1.0;
                    *result += sign * A->matrix[0][j] * minor_det;
                }
            } else {
                status = CALCULATION_ERROR;
            }
        }
    }

    return status;
}