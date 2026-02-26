#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "../s21_matrix.h"

#define EPS 1e-7

static int g_wrap_calc_complements_fail = 0;
static int g_wrap_transpose_fail = 0;
static int g_wrap_mult_number_fail = 0;
static int g_wrap_determinant_fail = 0;
static int g_wrap_create_matrix_internal_fail = 0;
static int g_wrap_get_minor_internal_fail = 0;

int __real_s21_calc_complements(matrix_t *A, matrix_t *result);
int __real_s21_transpose(matrix_t *A, matrix_t *result);
int __real_s21_mult_number(matrix_t *A, double number, matrix_t *result);
int __real_s21_determinant(matrix_t *A, double *result);
int __real_s21_create_matrix(int rows, int columns, matrix_t *result);
int __real_s21_get_minor(matrix_t *matrix, int row, int col, matrix_t *minor);

int __wrap_s21_calc_complements(matrix_t *A, matrix_t *result) {
    if (g_wrap_calc_complements_fail) {
        g_wrap_calc_complements_fail = 0;
        return 1;
    }
    return __real_s21_calc_complements(A, result);
}

int __wrap_s21_transpose(matrix_t *A, matrix_t *result) {
    if (g_wrap_transpose_fail) {
        g_wrap_transpose_fail = 0;
        return 1;
    }
    return __real_s21_transpose(A, result);
}

int __wrap_s21_mult_number(matrix_t *A, double number, matrix_t *result) {
    if (g_wrap_mult_number_fail) {
        g_wrap_mult_number_fail = 0;
        return 1;
    }
    return __real_s21_mult_number(A, number, result);
}

int __wrap_s21_determinant(matrix_t *A, double *result) {
    if (g_wrap_determinant_fail) {
        g_wrap_determinant_fail = 0;
        return 1;
    }
    return __real_s21_determinant(A, result);
}

int __wrap_s21_create_matrix(int rows, int columns, matrix_t *result) {
    if (g_wrap_create_matrix_internal_fail) {
        g_wrap_create_matrix_internal_fail = 0;
        return 1;
    }
    return __real_s21_create_matrix(rows, columns, result);
}

int __wrap_s21_get_minor(matrix_t *matrix, int row, int col, matrix_t *minor) {
    if (g_wrap_get_minor_internal_fail) {
        g_wrap_get_minor_internal_fail = 0;
        return 1;
    }
    return __real_s21_get_minor(matrix, row, col, minor);
}

static int g_calloc_fail_after = -1;
static int g_calloc_call_count = 0;
static void *(*__real_calloc_ptr)(size_t, size_t) = NULL;

void *__wrap_calloc(size_t nmemb, size_t size) {
    if (__real_calloc_ptr == NULL) {
        union {
            void *obj;
            void *(*func)(size_t, size_t);
        } converter;
        converter.obj = dlsym(RTLD_NEXT, "calloc");
        __real_calloc_ptr = converter.func;
    }
    g_calloc_call_count++;
    if (g_calloc_fail_after > 0 && g_calloc_call_count == g_calloc_fail_after) {
        return NULL;
    }
    return __real_calloc_ptr(nmemb, size);
}

void fill_matrix(matrix_t *A, double values[]) {
    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
            A->matrix[i][j] = values[i * A->columns + j];
        }
    }
}

START_TEST(test_create_matrix_success) {
    matrix_t A;
    ck_assert_int_eq(s21_create_matrix(3, 4, &A), 0);
    ck_assert_int_eq(A.rows, 3);
    ck_assert_int_eq(A.columns, 4);
    ck_assert_ptr_ne(A.matrix, NULL);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_create_matrix_invalid_size) {
    matrix_t A;
    ck_assert_int_eq(s21_create_matrix(0, 3, &A), 1);
    ck_assert_int_eq(s21_create_matrix(3, 0, &A), 1);
    ck_assert_int_eq(s21_create_matrix(-1, 3, &A), 1);
}
END_TEST

START_TEST(test_create_matrix_null_ptr) {
    ck_assert_int_eq(s21_create_matrix(3, 3, NULL), 1);
}
END_TEST

START_TEST(test_create_matrix_calloc_row_fail) {
    matrix_t A;
    g_calloc_call_count = 0;
    g_calloc_fail_after = 2;
    ck_assert_int_eq(s21_create_matrix(3, 3, &A), 1);
    ck_assert_ptr_eq(A.matrix, NULL);
    g_calloc_fail_after = -1;
}
END_TEST

START_TEST(test_remove_matrix_success) {
    matrix_t A;
    s21_create_matrix(3, 3, &A);
    s21_remove_matrix(&A);
    ck_assert_ptr_eq(A.matrix, NULL);
}
END_TEST

START_TEST(test_remove_matrix_null_ptr) {
    s21_remove_matrix(NULL);
}
END_TEST

START_TEST(test_remove_matrix_already_freed) {
    matrix_t A;
    s21_create_matrix(2, 2, &A);
    s21_remove_matrix(&A);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_eq_matrix_equal) {
    matrix_t A, B;
    double vals[] = {1, 2, 3, 4};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    fill_matrix(&A, vals);
    fill_matrix(&B, vals);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different) {
    matrix_t A, B;
    double valsA[] = {1, 2, 3, 4};
    double valsB[] = {1, 2, 3, 5};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    fill_matrix(&A, valsA);
    fill_matrix(&B, valsB);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), 0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different_size) {
    matrix_t A, B;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 3, &B);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), 0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_null_ptr) {
    matrix_t A;
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_eq_matrix(NULL, &A), 0);
    ck_assert_int_eq(s21_eq_matrix(&A, NULL), 0);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_sum_matrix_success) {
    matrix_t A, B, res, expected;
    double valsA[] = {1, 2, 3, 4};
    double valsB[] = {5, 6, 7, 8};
    double exp_vals[] = {6, 8, 10, 12};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    s21_create_matrix(2, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, valsA);
    fill_matrix(&B, valsB);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_sum_matrix_invalid_size) {
    matrix_t A, B, res;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 3, &B);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), 2);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sum_matrix_null_ptr) {
    matrix_t A, B, res;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    ck_assert_int_eq(s21_sum_matrix(NULL, &B, &res), 1);
    ck_assert_int_eq(s21_sum_matrix(&A, NULL, &res), 1);
    ck_assert_int_eq(s21_sum_matrix(&A, &B, NULL), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_success) {
    matrix_t A, B, res, expected;
    double valsA[] = {10, 20, 30, 40};
    double valsB[] = {1, 2, 3, 4};
    double exp_vals[] = {9, 18, 27, 36};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    s21_create_matrix(2, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, valsA);
    fill_matrix(&B, valsB);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_sub_matrix_invalid_size) {
    matrix_t A, B, res;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(3, 2, &B);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), 2);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_null_ptr_a) {
    matrix_t B, res;
    s21_create_matrix(2, 2, &B);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sub_matrix(NULL, &B, &res), 1);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_null_ptr_b) {
    matrix_t A, res;
    s21_create_matrix(2, 2, &A);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sub_matrix(&A, NULL, &res), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_null_ptr_result) {
    matrix_t A, B;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    ck_assert_int_eq(s21_sub_matrix(&A, &B, NULL), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_zero_rows_a) {
    matrix_t A, B, res;
    A.rows = 0; A.columns = 2; A.matrix = NULL;
    s21_create_matrix(2, 2, &B);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), 1);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_zero_cols_b) {
    matrix_t A, B, res;
    s21_create_matrix(2, 2, &A);
    B.rows = 2; B.columns = 0; B.matrix = NULL;
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_success) {
    matrix_t A, res, expected;
    double vals[] = {1, 2, 3, 4};
    double exp_vals[] = {2, 4, 6, 8};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_mult_number(&A, 2.0, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_number_null_ptr) {
    matrix_t A, res;
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_mult_number(NULL, 2.0, &res), 1);
    ck_assert_int_eq(s21_mult_number(&A, 2.0, NULL), 1);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_success) {
    matrix_t A, B, res, expected;
    double valsA[] = {1, 2, 3, 4};
    double valsB[] = {5, 6, 7, 8};
    double exp_vals[] = {19, 22, 43, 50};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    s21_create_matrix(2, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, valsA);
    fill_matrix(&B, valsB);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
    matrix_t A, B, res;
    s21_create_matrix(2, 3, &A);
    s21_create_matrix(2, 2, &B);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), 2);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose_success) {
    matrix_t A, res, expected;
    double vals[] = {1, 2, 3, 4, 5, 6};
    double exp_vals[] = {1, 4, 2, 5, 3, 6};
    s21_create_matrix(2, 3, &A);
    s21_create_matrix(3, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_transpose(&A, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_transpose_null_ptr) {
    matrix_t A, res;
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_transpose(NULL, &res), 1);
    ck_assert_int_eq(s21_transpose(&A, NULL), 1);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_1x1) {
    matrix_t A;
    double det;
    double vals[] = {5};
    s21_create_matrix(1, 1, &A);
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_determinant(&A, &det), 0);
    ck_assert_float_eq_tol(det, 5.0, EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
    matrix_t A;
    double det;
    double vals[] = {4, 7, 2, 6};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_determinant(&A, &det), 0);
    ck_assert_float_eq_tol(det, 10.0, EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
    matrix_t A;
    double det;
    double vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    s21_create_matrix(3, 3, &A);
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_determinant(&A, &det), 0);
    ck_assert_float_eq_tol(det, 0.0, EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_non_square) {
    matrix_t A;
    double det;
    s21_create_matrix(2, 3, &A);
    ck_assert_int_eq(s21_determinant(&A, &det), 1);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_null_ptr) {
    matrix_t A;
    double det;
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_determinant(NULL, &det), 1);
    ck_assert_int_eq(s21_determinant(&A, NULL), 1);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_3x3) {
    matrix_t A, res, expected;
    double vals[] = {1, 2, 3, 0, 1, 4, 5, 6, 0};
    double exp_vals[] = {-24, 20, -5, 18, -15, 4, 5, -4, 1};
    s21_create_matrix(3, 3, &A);
    s21_create_matrix(3, 3, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_calc_complements(&A, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
    matrix_t A, res;
    s21_create_matrix(2, 3, &A);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_calc_complements(&A, &res), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_create_matrix_fail) {
    matrix_t A, res;
    double vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    s21_create_matrix(3, 3, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_create_matrix_internal_fail = 1;
    ck_assert_int_eq(s21_calc_complements(&A, &res), 2);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_determinant_fail) {
    matrix_t A, res;
    double vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    s21_create_matrix(3, 3, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_determinant_fail = 1;
    ck_assert_int_eq(s21_calc_complements(&A, &res), 2);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_get_minor_fail) {
    matrix_t A, res;
    double vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    s21_create_matrix(3, 3, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_get_minor_internal_fail = 1;
    ck_assert_int_eq(s21_calc_complements(&A, &res), 2);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_get_minor_success) {
    matrix_t A, minor;
    double vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double exp_vals[] = {5, 6, 8, 9};
    s21_create_matrix(3, 3, &A);
    memset(&minor, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_get_minor(&A, 0, 0, &minor), 0);
    ck_assert_int_eq(minor.rows, 2);
    ck_assert_int_eq(minor.columns, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ck_assert_float_eq_tol(minor.matrix[i][j], exp_vals[i * 2 + j], EPS);
        }
    }
    s21_remove_matrix(&A);
    s21_remove_matrix(&minor);
}
END_TEST

START_TEST(test_get_minor_invalid_index) {
    matrix_t A, minor;
    s21_create_matrix(3, 3, &A);
    memset(&minor, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_get_minor(&A, 3, 0, &minor), 1);
    ck_assert_int_eq(s21_get_minor(&A, 0, 3, &minor), 1);
    ck_assert_int_eq(s21_get_minor(&A, -1, 0, &minor), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&minor);
}
END_TEST

START_TEST(test_get_minor_1x1) {
    matrix_t A, minor;
    double vals[] = {5};
    s21_create_matrix(1, 1, &A);
    memset(&minor, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_get_minor(&A, 0, 0, &minor), 0);
    ck_assert_int_eq(minor.rows, 1);
    ck_assert_int_eq(minor.columns, 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&minor);
}
END_TEST

START_TEST(test_inverse_2x2_success) {
    matrix_t A, res, expected;
    double vals[] = {4, 7, 2, 6};
    double exp_vals[] = {0.6, -0.7, -0.2, 0.4};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_3x3_success) {
    matrix_t A, res, expected;
    double vals[] = {2, 1, 1, 1, 2, 1, 1, 1, 2};
    double exp_vals[] = {0.75, -0.25, -0.25, -0.25, 0.75, -0.25, -0.25, -0.25, 0.75};
    s21_create_matrix(3, 3, &A);
    s21_create_matrix(3, 3, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, exp_vals);
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_identity) {
    matrix_t A, res, expected;
    double vals[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    s21_create_matrix(3, 3, &A);
    s21_create_matrix(3, 3, &expected);
    memset(&res, 0, sizeof(matrix_t));
    fill_matrix(&A, vals);
    fill_matrix(&expected, vals);
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 0);
    ck_assert_int_eq(s21_eq_matrix(&res, &expected), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_null_A) {
    matrix_t res;
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_inverse_matrix(NULL, &res), 1);
}
END_TEST

START_TEST(test_inverse_null_result) {
    matrix_t A;
    double vals[] = {1, 0, 0, 1};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    ck_assert_int_eq(s21_inverse_matrix(&A, NULL), 1);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_non_square) {
    matrix_t A, res;
    s21_create_matrix(2, 3, &A);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 1);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_singular) {
    matrix_t A, res;
    double vals[] = {1, 2, 2, 4};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 2);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_calc_complements_error) {
    matrix_t A, res;
    double vals[] = {4, 7, 2, 6};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_calc_complements_fail = 1;
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 1);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_transpose_error) {
    matrix_t A, res;
    double vals[] = {4, 7, 2, 6};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_transpose_fail = 1;
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 1);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_mult_number_error) {
    matrix_t A, res;
    double vals[] = {4, 7, 2, 6};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_mult_number_fail = 1;
    ck_assert_int_eq(s21_inverse_matrix(&A, &res), 1);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_determinant_error) {
    matrix_t A, res;
    double vals[] = {4, 7, 2, 6};
    s21_create_matrix(2, 2, &A);
    fill_matrix(&A, vals);
    memset(&res, 0, sizeof(matrix_t));
    g_wrap_determinant_fail = 1;
    int status = s21_inverse_matrix(&A, &res);
    ck_assert_int_eq(status, 2);
    ck_assert_ptr_eq(res.matrix, NULL);
    s21_remove_matrix(&A);
    s21_remove_matrix(&res);
}
END_TEST

Suite *s21_matrix_suite(void) {
    Suite *s = suite_create("s21_matrix");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_create_matrix_success);
    tcase_add_test(tc_core, test_create_matrix_invalid_size);
    tcase_add_test(tc_core, test_create_matrix_null_ptr);
    tcase_add_test(tc_core, test_create_matrix_calloc_row_fail);
    tcase_add_test(tc_core, test_remove_matrix_success);
    tcase_add_test(tc_core, test_remove_matrix_null_ptr);
    tcase_add_test(tc_core, test_remove_matrix_already_freed);
    tcase_add_test(tc_core, test_eq_matrix_equal);
    tcase_add_test(tc_core, test_eq_matrix_different);
    tcase_add_test(tc_core, test_eq_matrix_different_size);
    tcase_add_test(tc_core, test_eq_matrix_null_ptr);
    tcase_add_test(tc_core, test_sum_matrix_success);
    tcase_add_test(tc_core, test_sum_matrix_invalid_size);
    tcase_add_test(tc_core, test_sum_matrix_null_ptr);
    tcase_add_test(tc_core, test_sub_matrix_success);
    tcase_add_test(tc_core, test_sub_matrix_invalid_size);
    tcase_add_test(tc_core, test_sub_matrix_null_ptr_a);
    tcase_add_test(tc_core, test_sub_matrix_null_ptr_b);
    tcase_add_test(tc_core, test_sub_matrix_null_ptr_result);
    tcase_add_test(tc_core, test_sub_matrix_zero_rows_a);
    tcase_add_test(tc_core, test_sub_matrix_zero_cols_b);
    tcase_add_test(tc_core, test_mult_number_success);
    tcase_add_test(tc_core, test_mult_number_null_ptr);
    tcase_add_test(tc_core, test_mult_matrix_success);
    tcase_add_test(tc_core, test_mult_matrix_incompatible);
    tcase_add_test(tc_core, test_transpose_success);
    tcase_add_test(tc_core, test_transpose_null_ptr);
    tcase_add_test(tc_core, test_determinant_1x1);
    tcase_add_test(tc_core, test_determinant_2x2);
    tcase_add_test(tc_core, test_determinant_3x3);
    tcase_add_test(tc_core, test_determinant_non_square);
    tcase_add_test(tc_core, test_determinant_null_ptr);
    tcase_add_test(tc_core, test_calc_complements_3x3);
    tcase_add_test(tc_core, test_calc_complements_non_square);
    tcase_add_test(tc_core, test_calc_complements_create_matrix_fail);
    tcase_add_test(tc_core, test_calc_complements_determinant_fail);
    tcase_add_test(tc_core, test_calc_complements_get_minor_fail);
    tcase_add_test(tc_core, test_get_minor_success);
    tcase_add_test(tc_core, test_get_minor_invalid_index);
    tcase_add_test(tc_core, test_get_minor_1x1);
    tcase_add_test(tc_core, test_inverse_2x2_success);
    tcase_add_test(tc_core, test_inverse_3x3_success);
    tcase_add_test(tc_core, test_inverse_identity);
    tcase_add_test(tc_core, test_inverse_null_A);
    tcase_add_test(tc_core, test_inverse_null_result);
    tcase_add_test(tc_core, test_inverse_non_square);
    tcase_add_test(tc_core, test_inverse_singular);
    tcase_add_test(tc_core, test_inverse_calc_complements_error);
    tcase_add_test(tc_core, test_inverse_transpose_error);
    tcase_add_test(tc_core, test_inverse_mult_number_error);
    tcase_add_test(tc_core, test_inverse_determinant_error);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = s21_matrix_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}