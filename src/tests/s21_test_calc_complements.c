#include <check.h>
#include <math.h>
#include <stdlib.h>
#include "../s21_matrix.h"

#define EPS 1e-7

START_TEST(test_calc_complements_1x1) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(1, 1, &A);
    A.matrix[0][0] = 5.0;
    
    ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
    ck_assert_int_eq(result.rows, 1);
    ck_assert_int_eq(result.columns, 1);
    ck_assert(fabs(result.matrix[0][0] - 1.0) < EPS);
    
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
    matrix_t A = {0}, result = {0}, expected = {0};
    s21_create_matrix(2, 2, &A);
    A.matrix[0][0] = 1; A.matrix[0][1] = 2;
    A.matrix[1][0] = 3; A.matrix[1][1] = 4;
    
    ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
    
    s21_create_matrix(2, 2, &expected);
    expected.matrix[0][0] = 4.0;  expected.matrix[0][1] = -3.0;
    expected.matrix[1][0] = -2.0; expected.matrix[1][1] = 1.0;
    
    ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
    
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
    s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_calc_complements_3x3) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(3, 3, &A);
    A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
    A.matrix[1][0] = 0; A.matrix[1][1] = 4; A.matrix[1][2] = 5;
    A.matrix[2][0] = 1; A.matrix[2][1] = 0; A.matrix[2][2] = 6;
    
    ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
    
    ck_assert_int_eq(result.rows, 3);
    ck_assert_int_eq(result.columns, 3);
    ck_assert(fabs(result.matrix[0][0] - 24.0) < EPS);   // +det([[4,5],[0,6]]) = 24
    ck_assert(fabs(result.matrix[0][1] - 5.0) < EPS);     // -det([[0,5],[1,6]]) = -(-5) = 5
    ck_assert(fabs(result.matrix[0][2] - (-4.0)) < EPS);  // +det([[0,4],[1,0]]) = -4
    ck_assert(fabs(result.matrix[1][1] - 3.0) < EPS);     // +det([[1,3],[1,6]]) = 3
    
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_null_A) {
    matrix_t result = {0};
    s21_create_matrix(2, 2, &result);
    ck_assert_int_eq(s21_calc_complements(NULL, &result), INCORRECT_MATRIX);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_null_result) {
    matrix_t A = {0};
    s21_create_matrix(2, 2, &A);
    ck_assert_int_eq(s21_calc_complements(&A, NULL), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
    matrix_t A = {0}, result = {0};
    s21_create_matrix(2, 3, &A);
    ck_assert_int_eq(s21_calc_complements(&A, &result), INCORRECT_MATRIX);
    s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_empty_matrix) {
    matrix_t A = {0}, result = {0};
    A.matrix = NULL;
    A.rows = 0;
    A.columns = 0;
    ck_assert_int_eq(s21_calc_complements(&A, &result), INCORRECT_MATRIX);
}
END_TEST

Suite *calc_complements_suite(void) {
    Suite *s = suite_create("calc_complements");
    TCase *tc_valid = tcase_create("valid");
    TCase *tc_invalid = tcase_create("invalid");
    
    tcase_add_test(tc_valid, test_calc_complements_1x1);
    tcase_add_test(tc_valid, test_calc_complements_2x2);
    tcase_add_test(tc_valid, test_calc_complements_3x3);
    
    tcase_add_test(tc_invalid, test_calc_complements_null_A);
    tcase_add_test(tc_invalid, test_calc_complements_null_result);
    tcase_add_test(tc_invalid, test_calc_complements_non_square);
    tcase_add_test(tc_invalid, test_calc_complements_empty_matrix);
    
    suite_add_tcase(s, tc_valid);
    suite_add_tcase(s, tc_invalid);
    return s;
}

int main(void) {
    Suite *s = calc_complements_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}