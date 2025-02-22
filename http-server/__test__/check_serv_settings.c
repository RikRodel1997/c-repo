#ifdef __linux__
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../serv_settings.h"

START_TEST(test_create_addr_in) {
    addr_in addr = create_addr_in(9999);

    char actual_addr[INET_ADDRSTRLEN];
    const char* expected_addr = "0.0.0.0";
    inet_ntop(AF_INET, &addr.sin_addr, actual_addr, sizeof(actual_addr));

    ck_assert_str_eq(actual_addr, expected_addr);
    ck_assert_int_eq(htons(addr.sin_port), 9999);
    ck_assert_int_eq(htons(addr.sin_family), 512);
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* core;

    s = suite_create("Server Settings");
    core = tcase_create("Core");

    tcase_add_test(core, test_create_addr_in);

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
