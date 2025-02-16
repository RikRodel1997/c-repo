#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../vm.h"
#include "mocks.h"

START_TEST(test_run_OP_RETURN) {
    init_vm();

    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN, 1);

    InterpretResult result = interpret(&chunk);
    free_vm();

    ck_assert_int_eq(result, INTERPRET_OK);
}
END_TEST

START_TEST(test_run_OP_CONSTANT) {
    init_vm();

    Chunk chunk;
    init_chunk(&chunk);
    int constant = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    InterpretResult result = interpret(&chunk);

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    printf("buf %s\n", buf);
    free_vm();
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("VM");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_run_OP_RETURN);
    // tcase_add_test(tc_core, test_run_OP_CONSTANT);

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
