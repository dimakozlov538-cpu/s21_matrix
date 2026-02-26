// tests/s21_test_remove_matrix.c
#include <check.h>
#include "../s21_matrix.h"
#include <stdlib.h>

START_TEST(test_remove_matrix_normal) {
    matrix_t m = {0};
    s21_create_matrix(2, 2, &m);
    s21_remove_matrix(&m);
    ck_assert_ptr_null(m.matrix);
    ck_assert_int_eq(m.rows, 0);
    ck_assert_int_eq(m.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_null) {
    s21_remove_matrix(NULL); // should not crash
}
END_TEST

Suite *remove_suite(void) {
    Suite *s = suite_create("remove");
    TCase *tc = tcase_create("core");
    tcase_add_test(tc, test_remove_matrix_normal);
    tcase_add_test(tc, test_remove_matrix_null);
    suite_add_tcase(s, tc);
    return s;
}

// 🔑 ОБЯЗАТЕЛЬНО: функция main!
int main(void) {
    Suite *s = remove_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}