#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_eq_matrix_null_A) {
    matrix_t B = {0};
    s21_create_matrix(1, 1, &B);
    ck_assert_int_eq(s21_eq_matrix(NULL, &B), FAILURE);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_null_B) {
    matrix_t A = {0};
    s21_create_matrix(1, 1, &A);
    ck_assert_int_eq(s21_eq_matrix(&A, NULL), FAILURE);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_eq_matrix_both_null) {
    ck_assert_int_eq(s21_eq_matrix(NULL, NULL), FAILURE);
}
END_TEST

START_TEST(test_eq_matrix_different_rows) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(3, 2, &B);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_different_columns) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 3, &B);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_equal_simple) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);
    A.matrix[0][0] = 1.0;
    A.matrix[0][1] = 2.5;
    A.matrix[1][0] = -3.141592;
    A.matrix[1][1] = 0.0;

    B.matrix[0][0] = 1.0;
    B.matrix[0][1] = 2.5;
    B.matrix[1][0] = -3.141593; // отличие на 0.000001 → допустимо
    B.matrix[1][1] = 0.0;

    ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_not_equal) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(1, 1, &A);
    s21_create_matrix(1, 1, &B);
    A.matrix[0][0] = 1.000001;
    B.matrix[0][0] = 1.000003; // разница = 0.000002 > 1e-6
    ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_zero_matrices) {
    matrix_t A = {0}, B = {0};
    s21_create_matrix(3, 3, &A);
    s21_create_matrix(3, 3, &B);
    ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

Suite *eq_suite(void) {
    Suite *s = suite_create("eq_matrix");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_eq_matrix_null_A);
    tcase_add_test(tc, test_eq_matrix_null_B);
    tcase_add_test(tc, test_eq_matrix_both_null);
    tcase_add_test(tc, test_eq_matrix_different_rows);
    tcase_add_test(tc, test_eq_matrix_different_columns);
    tcase_add_test(tc, test_eq_matrix_equal_simple);
    tcase_add_test(tc, test_eq_matrix_not_equal);
    tcase_add_test(tc, test_eq_matrix_zero_matrices);

    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = eq_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}