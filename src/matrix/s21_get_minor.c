#include "s21_matrix.h"

/**
 * Формирует минор матрицы — подматрицу без указанной строки и столбца
 * 
 * @param matrix Исходная матрица
 * @param row    Индекс удаляемой строки (0-based)
 * @param col    Индекс удаляемого столбца (0-based)
 * @param minor  Выходная матрица-минор (память выделяется внутри)
 * @return       OK — успех
 *               INCORRECT_MATRIX — некорректные входные данные
 *               CALCULATION_ERROR — ошибка выделения памяти
 */
int s21_get_minor(matrix_t *matrix, int row, int col, matrix_t *minor) {
    // Валидация входных данных
    if (!matrix || !minor || !matrix->matrix || matrix->rows <= 0 || matrix->columns <= 0 ||
        row < 0 || row >= matrix->rows || col < 0 || col >= matrix->columns) {
        return INCORRECT_MATRIX;
    }

    int new_rows = matrix->rows - 1;
    int new_cols = matrix->columns - 1;

    // Особый случай: для матрицы 1×1 минор по определению равен 1.0
    if (new_rows <= 0 || new_cols <= 0) {
        if (s21_create_matrix(1, 1, minor) != OK) {
            return CALCULATION_ERROR;
        }
        minor->matrix[0][0] = 1.0;
        return OK;
    }

    // Создаём матрицу для минора
    if (s21_create_matrix(new_rows, new_cols, minor) != OK) {
        return CALCULATION_ERROR;
    }

    // Копируем элементы, пропуская указанную строку и столбец
    int dest_row = 0;
    for (int src_row = 0; src_row < matrix->rows; ++src_row) {
        if (src_row == row) continue;

        int dest_col = 0;
        for (int src_col = 0; src_col < matrix->columns; ++src_col) {
            if (src_col == col) continue;
            minor->matrix[dest_row][dest_col++] = matrix->matrix[src_row][src_col];
        }
        ++dest_row;
    }

    return OK;
}