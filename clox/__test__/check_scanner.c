#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../scanner.h"

START_TEST(test_single_char_scans) {
    char* tokens[11] = {"(", ")", "{", "}", ",", ".", "-", "+", ";", "/", "*"};
    int expected_types[11] = {
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE, TOKEN_COMMA, TOKEN_DOT,
        TOKEN_MINUS,      TOKEN_PLUS,        TOKEN_SEMICOLON,  TOKEN_SLASH,       TOKEN_STAR,
    };

    for (int i = 0; i != 11; i++) {
        if (tokens[i] == NULL) {
            return;
        }
        init_scanner(tokens[i]);
        Token tk = scan_token();
        ck_assert_int_eq(tk.length, 1);
        ck_assert_int_eq(tk.line, 1);
        ck_assert_int_eq(tk.type, expected_types[i]);
        Token tk_eof = scan_token();
        ck_assert_int_eq(tk_eof.type, TOKEN_EOF);
    }
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Scanner");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_single_char_scans);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    SRunner* sr;
    sr = srunner_create(suite());
    srunner_run_all(sr, CK_NORMAL);
    srunner_ntests_failed(sr);
    srunner_free(sr);
}
