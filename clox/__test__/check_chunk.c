#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../chunk.h"

START_TEST(test_init_chunk) {
    Chunk chunk;
    init_chunk(&chunk);
    ck_assert_int_eq(chunk.count, 0);
    ck_assert_int_eq(chunk.capacity, 0);
    ck_assert_ptr_null(chunk.code);
}
END_TEST

START_TEST(test_write_chunk_1_byte) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN, 1);
    ck_assert_int_eq(chunk.count, 1);
    ck_assert_int_eq(chunk.capacity, 8);
    ck_assert_int_eq(chunk.code[0], 2);   // OP_RETURN (index in enum)
}
END_TEST

START_TEST(test_write_chunk_9_bytes) {
    Chunk chunk;
    init_chunk(&chunk);
    for (int i = 0; i < 9; i++) {
        write_chunk(&chunk, i, i);
    }
    ck_assert_int_eq(chunk.count, 9);
    ck_assert_int_eq(chunk.capacity, 16);
}
END_TEST

START_TEST(test_free_chunk) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, 1, 1);
    free_chunk(&chunk);
    ck_assert_int_eq(chunk.count, 0);
    ck_assert_int_eq(chunk.capacity, 0);
}
END_TEST

START_TEST(test_add_constant) {
    Chunk chunk;
    init_chunk(&chunk);
    Value value = 1.0;
    int pos = add_constant(&chunk, value);
    ck_assert_int_eq(chunk.count, 0);
    ck_assert_int_eq(pos, 0);
    ck_assert_int_eq(chunk.constants.count, 1);
    ck_assert_double_eq(chunk.constants.values[0], 1.0);
}
END_TEST

START_TEST(test_write_constant_smol_num) {
    Chunk chunk;
    init_chunk(&chunk);
    write_constant(&chunk, 1.0, 1);
    ck_assert_int_eq(chunk.count, 2);
    ck_assert_int_eq(chunk.capacity, 8);
    ck_assert_int_eq(chunk.code[0], 0);   // OP_CONSTANT (index in enum)
}
END_TEST

START_TEST(test_write_constant_big_num) {
    Chunk chunk;
    init_chunk(&chunk);
    for (int i = 0; i < 256; i++) {
        write_constant(&chunk, (double) i, 1);
    }

    write_constant(&chunk, 999.0, 1);
    int long_constant_idx = chunk.code[chunk.count - 4];
    ck_assert_int_eq(chunk.count, 516);       // 4 bytes for OP_CONSTANT_LONG
    ck_assert_int_eq(long_constant_idx, 1);   // OP_CONSTANT_LONG (index in enum)
}
END_TEST

START_TEST(test_get_line) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_CONSTANT, 1);
    write_chunk(&chunk, 0, 1);
    write_chunk(&chunk, OP_RETURN, 2);
    ck_assert_int_eq(get_line(&chunk, 0), 1);
    ck_assert_int_eq(get_line(&chunk, 1), 1);
    ck_assert_int_eq(get_line(&chunk, 2), 2);
}
END_TEST

// binary search stats in the middle of the array
START_TEST(test_get_line_instruction_less_than_offset) {
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_CONSTANT, 1);
    write_chunk(&chunk, 0, 1);
    write_chunk(&chunk, OP_CONSTANT, 2);
    write_chunk(&chunk, 1, 2);
    write_chunk(&chunk, OP_RETURN, 3);
    ck_assert_int_eq(get_line(&chunk, 0), 1);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Chunk");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_init_chunk);
    tcase_add_test(tc_core, test_write_chunk_1_byte);
    tcase_add_test(tc_core, test_write_chunk_9_bytes);
    tcase_add_test(tc_core, test_free_chunk);
    tcase_add_test(tc_core, test_add_constant);
    tcase_add_test(tc_core, test_get_line);
    tcase_add_test(tc_core, test_get_line_instruction_less_than_offset);
    tcase_add_test(tc_core, test_write_constant_smol_num);
    tcase_add_test(tc_core, test_write_constant_big_num);

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
