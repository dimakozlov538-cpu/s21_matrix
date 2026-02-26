#include "s21_matrix.h"
#include <math.h>

int s21_determinant(matrix_t *A, double *result) {
    // Валидация входных данных
    if (!A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0 || 
        A->rows != A->columns) {
        return INCORRECT_MATRIX;
    }

    // Базовый случай 1×1
    if (A->rows == 1) {
        *result = A->matrix[0][0];
        return OK;
    }

    // Базовый случай 2×2
    if (A->rows == 2) {
        *result = A->matrix[0][0] * A->matrix[1][1] - 
                  A->matrix[0][1] * A->matrix[1][0];
        return OK;
    }

    // Рекурсивное разложение по первой строке для матриц ≥ 3×3
    *result = 0.0;
    for (int j = 0; j < A->columns; ++j) {
        matrix_t minor = {0};
        if (s21_get_minor(A, 0, j, &minor) != OK) {
            return CALCULATION_ERROR;
        }

        double minor_det = 0.0;
        int status = s21_determinant(&minor, &minor_det);
        s21_remove_matrix(&minor);  // Освобождаем память сразу после использования

        if (status != OK) {
            return CALCULATION_ERROR;
        }

        // (-1)^(0+j) = (-1)^j → знак зависит от чётности j
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        *result += sign * A->matrix[0][j] * minor_det;
    }

    return OK;
}