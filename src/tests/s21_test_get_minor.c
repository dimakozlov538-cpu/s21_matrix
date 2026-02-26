#include <check.h>
#include <math.h>
#include <stdlib.h>
#include "../s21_matrix.h"

#define EPS 1e-7

START_TEST(test_get_minor_valid_3x3) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(3, 3, &A);
    A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
    A.matrix[1][0] = 4; A.matrix[1][1] = 5; A.matrix[1][2] = 6;
    A.matrix[2][0] = 7; A.matrix[2][1] = 8; A.matrix[2][2] = 9;
    
    ck_assert_int_eq(s21_get_minor(&A, 1, 1, &minor), OK);
    ck_assert_int_eq(minor.rows, 2);
    ck_assert_int_eq(minor.columns, 2);
    ck_assert(fabs(minor.matrix[0][0] - 1.0) < EPS);
    ck_assert(fabs(minor.matrix[0][1] - 3.0) < EPS);
    ck_assert(fabs(minor.matrix[1][0] - 7.0) < EPS);
    ck_assert(fabs(minor.matrix[1][1] - 9.0) < EPS);
    
    s21_remove_matrix(&A);
    s21_remove_matrix(&minor);
}
END_TEST

START_TEST(test_get_minor_1x1) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(1, 1, &A);
    A.matrix[0][0] = 42.0;
    
    ck_assert_int_eq(s21_get_minor(&A, 0, 0, &minor), OK);
    ck_assert_int_eq(minor.rows, 1);
    ck_assert_int_eq(minor.columns, 1);
    ck_assert(fabs(minor.matrix[0][0] - 1.0) < EPS);
    
    s21_remove_matrix(&A);
    s21_remove_matrix(&minor);
}
END_TEST

START_TEST(test_get_minor_null_matrix) {
    matrix_t minor = {0};
    ck_assert_int_eq(s21_get_minor(NULL, 0, 0, &minor), INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_get_minor_null_minor) {
    matrix_t A = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_get_minor(&A, 0, 0, NULL), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_get_minor_invalid_row_negative) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_get_minor(&A, -1, 0, &minor), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_get_minor_invalid_row_out_of_bounds) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_get_minor(&A, 2, 0, &minor), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_get_minor_invalid_col_negative) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_get_minor(&A, 0, -1, &minor), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_get_minor_invalid_col_out_of_bounds) {
    matrix_t A = {0}, minor = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_get_minor(&A, 0, 2, &minor), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_get_minor_empty_matrix) {
    matrix_t A = {0}, minor = {0};
    A.matrix = NULL;
    A.rows = 0;
    A.columns = 0;
    ck_assert_int_eq(s21_get_minor(&A, 0, 0, &minor), INCORRECT_MATRIX);
}
END_TEST

Suite *get_minor_suite(void) {
    Suite *s = suite_create("get_minor");
    TCase *tc_valid = tcase_create("valid");
    TCase *tc_invalid = tcase_create("invalid");
    
    tcase_add_test(tc_valid, test_get_minor_valid_3x3);
    tcase_add_test(tc_valid, test_get_minor_1x1);
    
    tcase_add_test(tc_invalid, test_get_minor_null_matrix);
    tcase_add_test(tc_invalid, test_get_minor_null_minor);
    tcase_add_test(tc_invalid, test_get_minor_invalid_row_negative);
    tcase_add_test(tc_invalid, test_get_minor_invalid_row_out_of_bounds);
    tcase_add_test(tc_invalid, test_get_minor_invalid_col_negative);
    tcase_add_test(tc_invalid, test_get_minor_invalid_col_out_of_bounds);
    tcase_add_test(tc_invalid, test_get_minor_empty_matrix);
    
    suite_add_tcase(s, tc_valid);
    suite_add_tcase(s, tc_invalid);
    return s;
}

int main(void) {
    Suite *s = get_minor_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}