#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../compiler.h"
#include "../scanner.h"

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Compiler");
    tc_core = tcase_create("Core");

    // TODO

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
