#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_sum_matrix_success) {
    matrix_t A = {0}, B = {0}, result = {0};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
    A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
    B.matrix[0][0] = 5.0; B.matrix[0][1] = 6.0;
    B.matrix[1][0] = 7.0; B.matrix[1][1] = 8.0;
    int code = s21_sum_matrix(&A, &B, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_double_eq(result.matrix[0][0], 6.0);
    ck_assert_double_eq(result.matrix[1][1], 12.0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_null) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(1, 1, &A);
    s21_create_matrix(1, 1, &B);
    int code = s21_sum_matrix(NULL, &B, NULL);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_size_mismatch) {
    matrix_t A = {0}, B = {0}, result = {0};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 3, &B);
    int code = s21_sum_matrix(&A, &B, &result);
    ck_assert_int_eq(code, CALCULATION_ERROR);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

Suite *sum_suite(void) {
    Suite *s = suite_create("sum");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_sum_matrix_success);
    tcase_add_test(tc, test_sum_matrix_null);
    tcase_add_test(tc, test_sum_matrix_size_mismatch);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = sum_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}