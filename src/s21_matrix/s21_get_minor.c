#include "s21_matrix.h"

static void copy_to_minor(matrix_t *matrix, matrix_t *minor, int skip_row, int skip_col) {
    int dest_row = 0;
    for (int src_row = 0; src_row < matrix->rows; ++src_row) {
        if (src_row == skip_row) {
            continue;
        }
        int dest_col = 0;
        for (int src_col = 0; src_col < matrix->columns; ++src_col) {
            if (src_col == skip_col) {
                continue;
            }
            minor->matrix[dest_row][dest_col] = matrix->matrix[src_row][src_col];
            ++dest_col;
        }
        ++dest_row;
    }
}

int s21_get_minor(matrix_t *matrix, int row, int col, matrix_t *minor) {
    int status = OK;
    
    if (!matrix || !minor || !matrix->matrix || matrix->rows <= 0 || matrix->columns <= 0) {
        return INCORRECT_MATRIX;
    }
    
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->columns) {
        return INCORRECT_MATRIX;
    }

    int new_rows = matrix->rows - 1;
    int new_cols = matrix->columns - 1;

    if (new_rows <= 0 || new_cols <= 0) {
        status = s21_create_matrix(1, 1, minor);
        if (status == OK) {
            minor->matrix[0][0] = 1.0;
        }
    } else {
        status = s21_create_matrix(new_rows, new_cols, minor);
        if (status == OK) {
            copy_to_minor(matrix, minor, row, col);
        }
    }

    return status;
}