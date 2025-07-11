#include <check.h>
#include <stdbool.h>

#include "../include/request.h"

START_TEST(test_parse_request_post_home) {
    Request req = parse_request("POST /home");
    ck_assert_str_eq(req.method, "POST");
    ck_assert_str_eq(req.path, "/home");
}
END_TEST

START_TEST(test_parse_request_invalid_method) {
    Request req = parse_request("TOOLONGMETHOD /home");
    ck_assert_str_eq(req.method, "");
    ck_assert_str_eq(req.path, "/invalid");
}
END_TEST

START_TEST(test_parse_request_protocol) {
    Request req = parse_request("POST /home HTTP/1.1");
    ck_assert_str_eq(req.method, "POST");
    ck_assert_str_eq(req.path, "/home");
    ck_assert_str_eq(req.protocol, "HTTP/1.1");
}
END_TEST

START_TEST(test_valid_method_true) {
    ck_assert_int_eq(valid_method("POST"), 1);
    ck_assert_int_eq(valid_method("GET"), 1);
    ck_assert_int_eq(valid_method("PUT"), 1);
    ck_assert_int_eq(valid_method("DELETE"), 1);
}
END_TEST

START_TEST(test_curl_user_agent) {
    char headers[] = "Host: localhost:4221 User-Agent: curl/7.81.0 Accept: */*";
    char curl_ua[50];
    parse_user_agent(headers, curl_ua);
    ck_assert_str_eq(curl_ua, "curl/7.81.0");
    ck_assert_int_eq(strlen(curl_ua), 11);
}
END_TEST

START_TEST(test_python_user_agent) {
    char headers[] = "Host: localhost:4221 Accept: */* User-Agent: Python/3.10 aiohttp/3.9.5";
    char curl_ua[50];
    parse_user_agent(headers, curl_ua);
    ck_assert_str_eq(curl_ua, "Python/3.10");
    ck_assert_int_eq(strlen(curl_ua), 11);
}
END_TEST

START_TEST(test_valid_method_false) { ck_assert_int_eq(valid_method("PATCH"), 0); }
END_TEST

START_TEST(test_echo_tail_exists) {
    char tail[6];
    get_echo_tail("/echo/TAIL12", tail);
    ck_assert_str_eq(tail, "TAIL12");
}
END_TEST

START_TEST(test_echo_tail_doesnt_exists) {
    char tail[6];
    get_echo_tail("/echo/", tail);
    ck_assert_str_eq(tail, "");
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Request");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parse_request_post_home);
    tcase_add_test(tc_core, test_parse_request_invalid_method);
    tcase_add_test(tc_core, test_parse_request_protocol);
    tcase_add_test(tc_core, test_valid_method_true);
    tcase_add_test(tc_core, test_valid_method_false);
    tcase_add_test(tc_core, test_echo_tail_exists);
    tcase_add_test(tc_core, test_echo_tail_doesnt_exists);
    tcase_add_test(tc_core, test_curl_user_agent);
    tcase_add_test(tc_core, test_python_user_agent);

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
