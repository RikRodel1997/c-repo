#!/bin/bash
if [ ! -d "clox/out" ]; then
  mkdir clox/out
fi

TEST_PATH="clox/__test__"
OUT_PATH="clox/out"
SRC_PATH="clox/src"

MOCK_LIBS="$TEST_PATH/mocks.c $TEST_PATH/mocks.h"
TEST_FLAGS="$(pkg-config --cflags --libs check) -fprofile-arcs -ftest-coverage -lgcov"

rm -f $OUT_PATH/*.gcda $OUT_PATH/*.gcno coverage.info coverage_filtered.info

gcc -O0 -o $OUT_PATH/check_chunk $TEST_PATH/check_chunk.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_chunk
gcc -DTESTING -O0 -o $OUT_PATH/check_memory $TEST_PATH/check_memory.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_memory
gcc -DTESTING -O0 -o $OUT_PATH/check_debug $TEST_PATH/check_debug.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_debug
gcc -DTESTING -O0 -o $OUT_PATH/check_value $TEST_PATH/check_value.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_value
gcc -DTESTING -O0 -o $OUT_PATH/check_scanner $TEST_PATH/check_scanner.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_scanner
gcc -DDEBUG_PRINT_CODE -O0 -o $OUT_PATH/check_compiler $TEST_PATH/check_compiler.c $(ls $SRC_PATH/*.c | grep -v 'vm.c') $MOCK_LIBS $TEST_FLAGS && $OUT_PATH/check_compiler

rm -f $OUT_PATH/*-mocks.*

lcov --capture --directory clox/out --output-file coverage_report/coverage.info -q > /dev/null 2>&1
lcov --remove coverage_report/coverage.info "*$TEST_PATH/*" --output-file coverage_report/coverage_filtered.info -q > /dev/null 2>&1
genhtml coverage_report/coverage_filtered.info --output-directory coverage_report -q > /dev/null 2>&1

