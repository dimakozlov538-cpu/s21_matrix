#include "../s21_matrix.h"
#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int code = OK;
    
    if (result == NULL || rows <= 0 || columns <= 0) {
        return INCORRECT_MATRIX;
    }
    
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL) {
        return INCORRECT_MATRIX;
    }
    
    for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result->matrix[j]);
            }
            free(result->matrix);
            result->matrix = NULL;
            code = INCORRECT_MATRIX;
            break;
        }
    }
    
    if (code == OK) {
        result->rows = rows;
        result->columns = columns;
    }
    
    return code;
}