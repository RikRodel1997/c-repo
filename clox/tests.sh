MOCK_LIBS="__test__/mocks.c __test__/mocks.h"
TEST_FLAGS="$(pkg-config --cflags --libs check) -fprofile-arcs -ftest-coverage -lgcov"

rm -f out/*.gcda 
rm -f out/*.gcno 
rm -f coverage.info coverage_filtered.info

gcc -O0 -o out/check_chunk __test__/check_chunk.c src/*.c $MOCK_LIBS $TEST_FLAGS
out/check_chunk
gcc -DTESTING -O0 -o out/check_memory __test__/check_memory.c src/*.c $MOCK_LIBS $TEST_FLAGS
out/check_memory
gcc -DTESTING -O0 -o out/check_debug __test__/check_debug.c src/*.c $MOCK_LIBS $TEST_FLAGS
out/check_debug
gcc -DTESTING -O0 -o out/check_value __test__/check_value.c src/*.c $MOCK_LIBS $TEST_FLAGS
out/check_value
gcc -DTESTING -O0 -o out/check_vm __test__/check_vm.c src/*.c $MOCK_LIBS $TEST_FLAGS
out/check_vm

lcov --capture --directory clox --output-file coverage_report/coverage.info -q 2>/dev/null
lcov --remove coverage_report/coverage.info "*__test__/*" --output-file coverage_report/coverage_filtered.info -q 2>/dev/null
lcov --list coverage_report/coverage_filtered.info -q 2>/dev/null
genhtml coverage_report/coverage_filtered.info --output-directory coverage_report -q 2>/dev/null

