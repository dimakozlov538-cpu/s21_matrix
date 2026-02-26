#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_transpose_success) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(2, 3, &A);
    A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0; A.matrix[0][2] = 3.0;
    A.matrix[1][0] = 4.0; A.matrix[1][1] = 5.0; A.matrix[1][2] = 6.0;
    int code = s21_transpose(&A, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(result.rows, 3);
    ck_assert_int_eq(result.columns, 2);
    ck_assert_double_eq(result.matrix[0][0], 1.0);
    ck_assert_double_eq(result.matrix[1][0], 2.0);
    ck_assert_double_eq(result.matrix[2][1], 6.0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_null) {
    matrix_t A = {0};
    s21_create_matrix(1, 1, &A);
    int code = s21_transpose(NULL, NULL);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_transpose_square) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(2, 2, &A);
    A.matrix[0][0] = 9.0; A.matrix[0][1] = 8.0;
    A.matrix[1][0] = 7.0; A.matrix[1][1] = 6.0;
    int code = s21_transpose(&A, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_double_eq(result.matrix[0][1], 7.0);
    ck_assert_double_eq(result.matrix[1][0], 8.0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

Suite *transpose_suite(void) {
    Suite *s = suite_create("transpose");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_transpose_success);
    tcase_add_test(tc, test_transpose_null);
    tcase_add_test(tc, test_transpose_square);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = transpose_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}