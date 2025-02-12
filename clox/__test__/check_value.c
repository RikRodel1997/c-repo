#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../value.h"
#include "mocks.h"

START_TEST(test_init_value_array) {
    ValueArray arr;
    init_value_array(&arr);

    ck_assert_int_eq(arr.count, 0);
    ck_assert_int_eq(arr.capacity, 0);
    ck_assert_ptr_null(arr.values);
}
END_TEST

START_TEST(test_write_value_array_1_value) {
    ValueArray arr;
    init_value_array(&arr);
    write_value_array(&arr, 1.0);

    ck_assert_double_eq(arr.count, 1);
    ck_assert_int_eq(arr.capacity, 8);
    ck_assert_double_eq(arr.values[0], 1.0);
}
END_TEST

START_TEST(test_write_value_array_9_values) {
    ValueArray arr;
    init_value_array(&arr);
    for (int i = 0; i < 9; i++) {
        write_value_array(&arr, i);
    }

    ck_assert_int_eq(arr.count, 9);
    ck_assert_int_eq(arr.capacity, 16);
}
END_TEST

START_TEST(test_write_value_array_2_values) {
    ValueArray arr;
    init_value_array(&arr);
    write_value_array(&arr, 1.0);

    ck_assert_int_eq(arr.count, 1);
    ck_assert_int_eq(arr.capacity, 8);
    ck_assert_double_eq(arr.values[0], 1.0);
    ck_assert_double_eq(arr.values[arr.count - 1], arr.values[0]);
}
END_TEST

START_TEST(test_free_value_array) {
    ValueArray arr;
    init_value_array(&arr);
    write_value_array(&arr, 1);
    free_value_array(&arr);

    ck_assert_int_eq(arr.count, 0);
    ck_assert_int_eq(arr.capacity, 0);
}
END_TEST

START_TEST(test_print_value) {
    FILE* test_out = tmpfile();
    int og_stdout = setup_stdout(test_out);

    print_value(1.0);

    char buf[256];
    read_stdout(test_out, og_stdout, buf);
    ck_assert_str_eq(buf, "1");
}
END_TEST

Suite* suite(void) {
    Suite* s;
    TCase* tc_core;

    s = suite_create("Value");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_init_value_array);
    tcase_add_test(tc_core, test_free_value_array);
    tcase_add_test(tc_core, test_write_value_array_1_value);
    tcase_add_test(tc_core, test_write_value_array_9_values);
    tcase_add_test(tc_core, test_write_value_array_2_values);
    tcase_add_test(tc_core, test_print_value);
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
