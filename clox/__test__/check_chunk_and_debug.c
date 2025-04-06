#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/chunk.h"
#include "../include/debug.h"
#include "mocks.h"

START_TEST(test_free_chunk) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, 1, 1);
    free_chunk(&chunk);
    ck_assert_int_eq(chunk.count, 0);
    ck_assert_int_eq(chunk.capacity, 0);
}
END_TEST

START_TEST(test_disassemble_instruction_unknown_opcode) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, 231, 0);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    disassemble_chunk(&chunk, "test chunk");

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "== test chunk ==\n0000    0 Unknown opcode 231");
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Chunk & Debug");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_free_chunk);
    tcase_add_test(tc_core, test_disassemble_instruction_unknown_opcode);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    SRunner* sr;
    sr = srunner_create(suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
}
