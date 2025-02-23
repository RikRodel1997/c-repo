#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../vm.h"
#include "mocks.h"

// These tests were only relevant before we had the compiler
// START_TEST(test_run_OP_RETURN) {
//     init_vm();

//     Chunk chunk;
//     init_chunk(&chunk);
//     write_chunk(&chunk, OP_RETURN, 1);

//     InterpretResult result = interpret(&chunk);
//     free_vm();

//     ck_assert_int_eq(result, INTERPRET_OK);
// }
// END_TEST

// START_TEST(test_run_binary_operators) {
//     init_vm();

//     Chunk chunk;
//     init_chunk(&chunk);

//     write_chunk(&chunk, OP_CONSTANT, 0);
//     write_chunk(&chunk, add_constant(&chunk, 1.2), 0);
//     write_chunk(&chunk, OP_CONSTANT, 0);
//     write_chunk(&chunk, add_constant(&chunk, 3), 0);
//     write_chunk(&chunk, OP_ADD, 0);

//     write_chunk(&chunk, OP_CONSTANT, 0);
//     write_chunk(&chunk, add_constant(&chunk, 1), 0);
//     write_chunk(&chunk, OP_SUBTRACT, 0);

//     write_chunk(&chunk, OP_CONSTANT, 0);
//     write_chunk(&chunk, add_constant(&chunk, 1.5), 0);
//     write_chunk(&chunk, OP_MULTIPLY, 0);

//     write_chunk(&chunk, OP_CONSTANT, 0);
//     write_chunk(&chunk, add_constant(&chunk, 2), 0);
//     write_chunk(&chunk, OP_DIVIDE, 0);

//     write_chunk(&chunk, OP_NEGATE, 0);
//     write_chunk(&chunk, OP_RETURN, 0);

//     InterpretResult result = interpret(&chunk);

//     free_vm();
// }
// END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("VM");
    tc_core = tcase_create("Core");

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
