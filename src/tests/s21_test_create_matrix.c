#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_create_matrix_success) {
    matrix_t m = {0};
    int code = s21_create_matrix(2, 3, &m);
    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(m.rows, 2);
    ck_assert_int_eq(m.columns, 3);
    ck_assert_ptr_nonnull(m.matrix);
    s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_null_result) {
    int code = s21_create_matrix(1, 1, NULL);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_zero_rows) {
    matrix_t m = {0};
    int code = s21_create_matrix(0, 5, &m);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_negative_columns) {
    matrix_t m = {0};
    int code = s21_create_matrix(3, -1, &m);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_create_matrix_min_valid) {
    matrix_t m = {0};
    int code = s21_create_matrix(1, 1, &m);
    ck_assert_int_eq(code, OK);
    s21_remove_matrix(&m);
}
END_TEST

// Тест для покрытия ветки calloc == NULL
START_TEST(test_create_matrix_oom) {
    matrix_t m = {0};
    // Запрашиваем заведомо невозможный размер
    int code = s21_create_matrix(1000000000, 1000000000, &m);
    ck_assert_int_eq(code, INCORRECT_MATRIX);
    ck_assert_ptr_null(m.matrix);
}
END_TEST

Suite *create_suite(void) {
    Suite *s = suite_create("create");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_create_matrix_success);
    tcase_add_test(tc, test_create_matrix_null_result);
    tcase_add_test(tc, test_create_matrix_zero_rows);
    tcase_add_test(tc, test_create_matrix_negative_columns);
    tcase_add_test(tc, test_create_matrix_min_valid);
    tcase_add_test(tc, test_create_matrix_oom);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}