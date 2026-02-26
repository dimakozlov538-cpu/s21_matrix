#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_mult_number_success) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(2, 2, &A);
    A.matrix[0][0] = 1.0; A.matrix[0][1] = 2.0;
    A.matrix[1][0] = 3.0; A.matrix[1][1] = 4.0;
    int code = s21_mult_number(&A, 2.5, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_double_eq(result.matrix[0][0], 2.5);
    ck_assert_double_eq(result.matrix[1][1], 10.0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_null) {
    matrix_t A = {0};
    s21_create_matrix(1, 1, &A);
    int code = s21_mult_number(NULL, 5.0, NULL);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_zero) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(1, 3, &A);
    A.matrix[0][0] = 10.0; A.matrix[0][1] = -5.0; A.matrix[0][2] = 0.0;
    int code = s21_mult_number(&A, 0.0, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_double_eq(result.matrix[0][0], 0.0);
    ck_assert_double_eq(result.matrix[0][1], 0.0);
    ck_assert_double_eq(result.matrix[0][2], 0.0);
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

Suite *mult_number_suite(void) {
    Suite *s = suite_create("mult_number");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_mult_number_success);
    tcase_add_test(tc, test_mult_number_null);
    tcase_add_test(tc, test_mult_number_zero);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = mult_number_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}