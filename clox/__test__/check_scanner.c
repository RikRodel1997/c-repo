#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/scanner.h"

START_TEST(test_single_char_scans) {
    char* tokens[15] = {"(", ")", "{", "}", ",", ".", "-", "+", ";", "/", "*", "!", "=", ">", "<"};
    int expected_types[15] = {TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE, TOKEN_COMMA,
                              TOKEN_DOT,        TOKEN_MINUS,       TOKEN_PLUS,       TOKEN_SEMICOLON,   TOKEN_SLASH,
                              TOKEN_STAR,       TOKEN_BANG,        TOKEN_EQUAL,      TOKEN_GREATER,     TOKEN_LESS};

    for (int i = 0; i != 15; i++) {
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

START_TEST(test_double_char_scans) {
    char* tokens[4] = {"!=", "==", ">=", "<="};
    int expected_types[4] = {
        TOKEN_BANG_EQUAL,
        TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER_EQUAL,
        TOKEN_LESS_EQUAL,
    };

    for (int i = 0; i != 4; i++) {
        if (tokens[i] == NULL) {
            return;
        }
        init_scanner(tokens[i]);
        Token tk = scan_token();
        ck_assert_int_eq(tk.length, 2);
        ck_assert_int_eq(tk.line, 1);
        ck_assert_int_eq(tk.type, expected_types[i]);
        Token tk_eof = scan_token();
        ck_assert_int_eq(tk_eof.type, TOKEN_EOF);
    }
}
END_TEST

START_TEST(test_unexpected_token) {
    init_scanner("~");
    Token tk = scan_token();
    ck_assert_int_eq(tk.length, 21);
    ck_assert_str_eq(tk.start, "Unexpected character.");
    ck_assert_int_eq(tk.line, 1);
    ck_assert_int_eq(tk.type, TOKEN_ERROR);
    Token tk_eof = scan_token();
    ck_assert_int_eq(tk_eof.type, TOKEN_EOF);
}
END_TEST

START_TEST(test_double_char_no_match) {
    init_scanner("!!");
    Token tk_bang = scan_token();
    ck_assert_int_eq(tk_bang.length, 1);
    ck_assert_int_eq(tk_bang.line, 1);
    ck_assert_int_eq(tk_bang.type, TOKEN_BANG);
    Token tk_bang_2 = scan_token();
    ck_assert_int_eq(tk_bang_2.type, TOKEN_BANG);
    Token tk_eof = scan_token();
    ck_assert_int_eq(tk_eof.type, TOKEN_EOF);
}
END_TEST

START_TEST(test_skipping) {
    init_scanner(" \r\t//\n//\0");
    scan_token();
    ck_assert_int_eq(scan_token().type, TOKEN_EOF);
}
END_TEST

START_TEST(test_string) {
    init_scanner("\"xst123\"");
    ck_assert_int_eq(scan_token().type, TOKEN_STRING);

    init_scanner("\"xst\n123");
    ck_assert_int_eq(scan_token().type, TOKEN_ERROR);
}
END_TEST

START_TEST(test_number) {
    char* nums[3] = {"12", "1", "1.1"};
    for (int i = 0; i < 3; i++) {
        init_scanner(nums[i]);
        ck_assert_int_eq(scan_token().type, TOKEN_NUMBER);
    }
}
END_TEST

START_TEST(test_identifier) {
    init_scanner("xt12314tewst");
    ck_assert_int_eq(scan_token().type, TOKEN_IDENTIFIER);
}
END_TEST

START_TEST(test_keywords) {
    char* keywords[16] = {"and", "class", "else",   "false", "for",  "fun",  "if",  "nil",
                          "or",  "print", "return", "super", "this", "true", "var", "while"};
    TokenType expected[16] = {
        TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE,   TOKEN_FALSE, TOKEN_FOR,  TOKEN_FUN,  TOKEN_IF,  TOKEN_NIL,
        TOKEN_OR,  TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS, TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
    };
    for (int i = 0; i < 16; i++) {
        char* keyword = keywords[i];
        TokenType expected_tokentype = expected[i];
        init_scanner(keyword);
        Token tk = scan_token();
        ck_assert_int_eq(tk.type, expected_tokentype);
    }
}
END_TEST

START_TEST(test_keywords_start) {
    init_scanner("t");
    Token tk_t = scan_token();
    ck_assert_int_eq(tk_t.type, TOKEN_IDENTIFIER);

    init_scanner("f");
    Token tk_f = scan_token();
    ck_assert_int_eq(tk_f.type, TOKEN_IDENTIFIER);
}
END_TEST

START_TEST(test_keywords_autocomplete) {
    init_scanner("anx");
    Token tk_and = scan_token();
    ck_assert_int_eq(tk_and.type, TOKEN_IDENTIFIER);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Scanner");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_single_char_scans);
    tcase_add_test(tc_core, test_double_char_scans);
    tcase_add_test(tc_core, test_unexpected_token);
    tcase_add_test(tc_core, test_double_char_no_match);
    tcase_add_test(tc_core, test_skipping);
    tcase_add_test(tc_core, test_string);
    tcase_add_test(tc_core, test_number);
    tcase_add_test(tc_core, test_identifier);
    tcase_add_test(tc_core, test_keywords);
    tcase_add_test(tc_core, test_keywords_start);
    tcase_add_test(tc_core, test_keywords_autocomplete);

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
