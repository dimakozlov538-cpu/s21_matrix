#include <check.h>
#include <math.h>
#include <stdlib.h>
#include "../s21_matrix.h"

#define EPS 1e-7

START_TEST(test_determinant_1x1_positive) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(1, 1, &A);
    A.matrix[0][0] = 7.5;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det - 7.5) < EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_1x1_negative) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(1, 1, &A);
    A.matrix[0][0] = -3.14;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det + 3.14) < EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_1x1_zero) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(1, 1, &A);
    A.matrix[0][0] = 0.0;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det) < EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2_simple) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(2, 2, &A);
    A.matrix[0][0] = 1; A.matrix[0][1] = 2;
    A.matrix[1][0] = 3; A.matrix[1][1] = 4;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det + 2.0) < EPS);  // 1*4 - 2*3 = -2
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2_identity) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(2, 2, &A);
    A.matrix[0][0] = 1; A.matrix[0][1] = 0;
    A.matrix[1][0] = 0; A.matrix[1][1] = 1;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det - 1.0) < EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3_zero_det) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(3, 3, &A);
    // Линейно зависимые строки
    A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
    A.matrix[1][0] = 2; A.matrix[1][1] = 4; A.matrix[1][2] = 6;
    A.matrix[2][0] = 3; A.matrix[2][1] = 6; A.matrix[2][2] = 9;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det) < EPS);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3_nonzero) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(3, 3, &A);
    // Определитель = -4 (проверено: 2*(0*5-2*1) - (-1)*(1*5-2*4) + 3*(1*1-0*4) = -4)
    A.matrix[0][0] = 2; A.matrix[0][1] = -1; A.matrix[0][2] = 3;
    A.matrix[1][0] = 1; A.matrix[1][1] = 0;  A.matrix[1][2] = 2;
    A.matrix[2][0] = 4; A.matrix[2][1] = 1;  A.matrix[2][2] = 5;
    ck_assert_int_eq(s21_determinant(&A, &det), OK);
    ck_assert(fabs(det + 4.0) < EPS);  // det = -4
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_null_matrix) {
    double det = 0.0;
    ck_assert_int_eq(s21_determinant(NULL, &det), INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_determinant_null_result) {
    matrix_t A = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_determinant(&A, NULL), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_non_square) {
    matrix_t A = {0};
    double det = 0.0;
    s21_create_matrix(2, 3, &A);
    ck_assert_int_eq(s21_determinant(&A, &det), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_empty_matrix) {
    matrix_t A = {0};
    double det = 0.0;
    A.matrix = NULL;
    A.rows = 0;
    A.columns = 0;
    ck_assert_int_eq(s21_determinant(&A, &det), INCORRECT_MATRIX);
}
END_TEST

Suite *determinant_suite(void) {
    Suite *s = suite_create("determinant");
    TCase *tc_valid = tcase_create("valid");
    TCase *tc_invalid = tcase_create("invalid");
    
    tcase_add_test(tc_valid, test_determinant_1x1_positive);
    tcase_add_test(tc_valid, test_determinant_1x1_negative);
    tcase_add_test(tc_valid, test_determinant_1x1_zero);
    tcase_add_test(tc_valid, test_determinant_2x2_simple);
    tcase_add_test(tc_valid, test_determinant_2x2_identity);
    tcase_add_test(tc_valid, test_determinant_3x3_zero_det);
    tcase_add_test(tc_valid, test_determinant_3x3_nonzero);
    
    tcase_add_test(tc_invalid, test_determinant_null_matrix);
    tcase_add_test(tc_invalid, test_determinant_null_result);
    tcase_add_test(tc_invalid, test_determinant_non_square);
    tcase_add_test(tc_invalid, test_determinant_empty_matrix);
    
    suite_add_tcase(s, tc_valid);
    suite_add_tcase(s, tc_invalid);
    return s;
}

int main(void) {
    Suite *s = determinant_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}