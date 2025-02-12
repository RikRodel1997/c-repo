#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../vm.h"
#include "mocks.h"

START_TEST(test_init_vm) { init_vm(); }
END_TEST

START_TEST(test_free_vm) {
    init_vm();
    free_vm();
}
END_TEST

START_TEST(test_run_OP_RETURN) {
    init_vm();

    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN, 1);

    InterpretResult result = interpret(&chunk);
    free_vm();

    ck_assert_int_eq(result, 0);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("VM");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_init_vm);
    tcase_add_test(tc_core, test_free_vm);
    tcase_add_test(tc_core, test_run_OP_RETURN);

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
