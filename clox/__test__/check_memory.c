#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/memory.h"
#include "mocks.h"

START_TEST(reallocate_new_allocation) { ck_assert_ptr_ne(reallocate(NULL, 0, 1), NULL); }
END_TEST

START_TEST(reallocate_free_allocation) {
    int* data = malloc(sizeof(int));
    *data = 42;

    void* result = reallocate(data, sizeof(int), 0);
    ck_assert_ptr_null(result);
    // checks that the pointer is not the same memory address, meaning it was freed
    ck_assert_ptr_ne(data, result);
}
END_TEST

START_TEST(reallocate_shrink_allocation) { ck_assert_ptr_ne(reallocate(NULL, 2, 1), NULL); }
END_TEST

START_TEST(reallocate_expand_allocation) { ck_assert_ptr_ne(reallocate(NULL, 1, 2), NULL); }
END_TEST

START_TEST(reallocate_realloc_failure) {
    void* ptr = malloc(10);

    if (setjmp(exit_buffer) == 0) {
        void* result = reallocate(ptr, 10, SIZE_MAX);
        ck_assert_msg(0, "Should not reach here");
    }

    ck_assert_int_eq(exit_code, 1);
    free(ptr);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Memory");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, reallocate_new_allocation);
    tcase_add_test(tc_core, reallocate_free_allocation);
    tcase_add_test(tc_core, reallocate_shrink_allocation);
    tcase_add_test(tc_core, reallocate_expand_allocation);
    tcase_add_test(tc_core, reallocate_realloc_failure);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    SRunner* sr;
    sr = srunner_create(suite());
    srunner_run_all(sr, CK_MINIMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
}
