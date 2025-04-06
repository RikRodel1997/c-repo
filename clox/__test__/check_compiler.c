#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/compiler.h"
#include "../include/scanner.h"
#include "mocks.h"

START_TEST(test_simple_binary_expressions) {
    char* expressions[4] = {"1 + 2", "1 - 2", "1 * 2", "1 / 2"};
    char* expected_ops[4] = {"OP_ADD", "OP_SUBTRACT", "OP_MULTIPLY", "OP_DIVIDE"};

    for (int i = 0; i < 4; i++) {
        Chunk chunk;
        init_chunk(&chunk);
        char* expr = expressions[i];
        char* op = expected_ops[i];

        FILE* test_out = tmpfile();
        int og_stdout = setup_stdout(test_out);

        bool result = compile(expr, &chunk);

        char buf[1024];
        read_stdout(test_out, og_stdout, buf);

        char expected_out[1024];
        snprintf(expected_out, sizeof(expected_out),
                 "== code ==\n"
                 "0000    1 OP_CONSTANT         0 '1'\n"
                 "0002    | OP_CONSTANT         1 '2'\n"
                 "0004    | %s\n"
                 "0005    | OP_RETURN",
                 op);

        ck_assert(result == true);
        ck_assert_int_eq(chunk.constants.count, 2);
        ck_assert_int_eq(AS_NUMBER(chunk.constants.values[0]), 1);
        ck_assert_int_eq(AS_NUMBER(chunk.constants.values[1]), 2);
    }
}
END_TEST

START_TEST(test_grouping_expression) {

    Chunk chunk;
    init_chunk(&chunk);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);
    bool result = compile("3 + (1 + 2)", &chunk);

    char buf[1024];
    read_stdout(test_out, og_stdout, buf);

    char* expected_out = "== code ==\n"
                         "0000    1 OP_CONSTANT         0 '3'\n"
                         "0002    | OP_CONSTANT         1 '1'\n"
                         "0004    | OP_CONSTANT         2 '2'\n"
                         "0006    | OP_ADD\n"
                         "0007    | OP_ADD\n"
                         "0008    | OP_RETURN";
    ck_assert(result == true);
    ck_assert_str_eq(buf, expected_out);
}
END_TEST

START_TEST(test_unary_negate_expression) {
    Chunk chunk;
    init_chunk(&chunk);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    bool result = compile("3 + (-1 + 2)", &chunk);

    char buf[1024];
    read_stdout(test_out, og_stdout, buf);

    char* expected_out = "== code ==\n"
                         "0000    1 OP_CONSTANT         0 '3'\n"
                         "0002    | OP_CONSTANT         1 '1'\n"
                         "0004    | OP_NEGATE\n"
                         "0005    | OP_CONSTANT         2 '2'\n"
                         "0007    | OP_ADD\n"
                         "0008    | OP_ADD\n"
                         "0009    | OP_RETURN";
    ck_assert(result == true);
    ck_assert_str_eq(buf, expected_out);
}
END_TEST

START_TEST(test_expression_err) {
    Chunk chunk;
    init_chunk(&chunk);

    FILE* test_out_err = tmpfile();
    int og_stderr = setup_stderr(test_out_err);

    compile("~", &chunk);

    char buf_err[1024];
    read_stderr(test_out_err, og_stderr, buf_err);

    char* expected_out_err = "[line 1] Error: Unexpected character.";
    ck_assert_str_eq(buf_err, expected_out_err);
}
END_TEST

START_TEST(test_eof) {
    Chunk chunk;
    init_chunk(&chunk);

    FILE* test_out = tmpfile();
    int og_stdout = setup_stderr(test_out);

    bool result = compile("", &chunk);

    char buf[1024];
    read_stderr(test_out, og_stdout, buf);

    char* expected_out = "[line 1] Error at end: Expect expression.";
    ck_assert(result == false);
    ck_assert_str_eq(buf, expected_out);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* core;

    s = suite_create("Compiler");
    core = tcase_create("Core");

    tcase_add_test(core, test_simple_binary_expressions);
    tcase_add_test(core, test_grouping_expression);
    tcase_add_test(core, test_unary_negate_expression);
    tcase_add_test(core, test_expression_err);
    tcase_add_test(core, test_eof);

    suite_add_tcase(s, core);
    return s;
}

int main(void) {
    SRunner* sr;
    sr = srunner_create(suite());
    srunner_run_all(sr, CK_NORMAL);
    srunner_ntests_failed(sr);
    srunner_free(sr);
}
