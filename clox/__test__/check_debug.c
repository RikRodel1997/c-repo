#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../chunk.h"
#include "../debug.h"
#include "mocks.h"

START_TEST(test_disassemble_instruction_OP_RETURN) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN, 0);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    disassemble_chunk(&chunk, "test chunk");

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "== test chunk ==\n0000    0 OP_RETURN");
}
END_TEST

START_TEST(test_disassemble_instruction_OP_CONSTANT) {
    Chunk chunk;
    init_chunk(&chunk);

    int constant = add_constant(&chunk, 1.0);
    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, constant, 0);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    disassemble_chunk(&chunk, "test chunk");

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "== test chunk ==\n0000    0 OP_CONSTANT         0 '1'");
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

START_TEST(test_disassemble_OP_CONSTANT_and_OP_RETURN) {
    Chunk chunk;
    init_chunk(&chunk);

    int constant = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);
    write_chunk(&chunk, OP_RETURN, 123);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    disassemble_chunk(&chunk, "test chunk");

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "== test chunk ==\n0000  123 OP_CONSTANT         0 '1.2'\n0002    | OP_RETURN");
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Debug");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_disassemble_instruction_OP_RETURN);
    tcase_add_test(tc_core, test_disassemble_instruction_OP_CONSTANT);
    tcase_add_test(tc_core, test_disassemble_instruction_unknown_opcode);
    tcase_add_test(tc_core, test_disassemble_OP_CONSTANT_and_OP_RETURN);

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
