#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../chunk.h"
#include "../debug.h"
#include "mocks.h"

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

START_TEST(test_disassemble_binary_operators) {
    Chunk chunk;
    init_chunk(&chunk);

    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, add_constant(&chunk, 1.2), 0);
    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, add_constant(&chunk, 3), 0);
    write_chunk(&chunk, OP_ADD, 0);

    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, add_constant(&chunk, 1), 0);
    write_chunk(&chunk, OP_SUBTRACT, 0);

    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, add_constant(&chunk, 1.5), 0);
    write_chunk(&chunk, OP_MULTIPLY, 0);

    write_chunk(&chunk, OP_CONSTANT, 0);
    write_chunk(&chunk, add_constant(&chunk, 2), 0);
    write_chunk(&chunk, OP_DIVIDE, 0);

    write_chunk(&chunk, OP_NEGATE, 0);
    write_chunk(&chunk, OP_RETURN, 0);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    disassemble_chunk(&chunk, "test chunk");

    char buf[1024];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "== test chunk ==\n0000    0 OP_CONSTANT         0 '1.2'\n0002    | OP_CONSTANT         1 "
                          "'3'\n0004    | OP_ADD\n0005    | OP_CONSTANT         2 '1'\n0007    | OP_SUBTRACT\n0008    "
                          "| OP_CONSTANT         3 '1.5'\n0010    | OP_MULTIPLY\n0011    | OP_CONSTANT         4 "
                          "'2'\n0013    | OP_DIVIDE\n0014    | OP_NEGATE\n0015    | OP_RETURN");
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Debug");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_disassemble_instruction_unknown_opcode);
    tcase_add_test(tc_core, test_disassemble_binary_operators);

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
