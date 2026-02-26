#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_mult_matrix_success) {
    matrix_t A = {0}, B = {0}, result = {0};
    s21_create_matrix(2, 3, &A);
    s21_create_matrix(3, 2, &B);
    A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
    A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
    B.matrix[0][0] = 7; B.matrix[0][1] = 8;
    B.matrix[1][0] = 9; B.matrix[1][1] = 10;
    B.matrix[2][0] = 11; B.matrix[2][1] = 12;
    int code = s21_mult_matrix(&A, &B, &result);
    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(result.rows, 2);
    ck_assert_int_eq(result.columns, 2);
    ck_assert_double_eq(result.matrix[0][0], 58.0);  // 1*7 + 2*9 + 3*11
    ck_assert_double_eq(result.matrix[1][1], 154.0); // 4*8 + 5*10 + 6*12
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_null) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(1, 1, &A);
    s21_create_matrix(1, 1, &B);
    int code = s21_mult_matrix(NULL, &B, NULL);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
    matrix_t A = {0}, B = {0}, result = {0};
    s21_create_matrix(2, 3, &A);
    s21_create_matrix(2, 2, &B);
    int code = s21_mult_matrix(&A, &B, &result);
    ck_assert_int_eq(code, CALCULATION_ERROR);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

Suite *mult_suite(void) {
    Suite *s = suite_create("mult");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_mult_matrix_success);
    tcase_add_test(tc, test_mult_matrix_null);
    tcase_add_test(tc, test_mult_matrix_incompatible);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = mult_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}