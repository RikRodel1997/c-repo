#!/bin/bash

if [ ! -d "http-server/out" ]; then
  mkdir http-server/out
fi

TEST_PATH="http-server/__test__"
OUT_PATH="http-server/out"
SRC_PATH="http-server/src"

TEST_FLAGS="$(pkg-config --cflags --libs check) -fprofile-arcs -ftest-coverage -lgcov"

rm -f $OUT_PATH/*.gcda $OUT_PATH/*.gcno coverage.info coverage_filtered.info

gcc -O0 -o $OUT_PATH/check_request $TEST_PATH/check_request.c $SRC_PATH/*.c $TEST_FLAGS && $OUT_PATH/check_request
gcc -O0 -o $OUT_PATH/check_serv_settings $TEST_PATH/check_serv_settings.c $SRC_PATH/*.c $TEST_FLAGS && $OUT_PATH/check_serv_settings

rm -f $OUT_PATH/*-mocks.*

lcov --capture --directory $OUT_PATH --output-file coverage_report/http-server-coverage.info -q > /dev/null 2>&1
lcov --remove coverage_report/http-server-coverage.info "*$TEST_PATH/*" --output-file coverage_report/coverage_filtered.info -q > /dev/null 2>&1
genhtml coverage_report/coverage_filtered.info --output-directory coverage_report -q > /dev/null 2>&1

