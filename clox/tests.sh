MOCK_LIBS="clox/__test__/mocks.c clox/__test__/mocks.h"
TEST_FLAGS="$(pkg-config --cflags --libs check) -fprofile-arcs -ftest-coverage -lgcov"

rm -f clox/out/*.gcda 
rm -f clox/out/*.gcno 
rm -f coverage.info coverage_filtered.info

gcc -O0 -o clox/out/check_chunk clox/__test__/check_chunk.c clox/src/*.c $MOCK_LIBS $TEST_FLAGS
clox/out/check_chunk
gcc -DTESTING -O0 -o clox/out/check_memory clox/__test__/check_memory.c clox/src/*.c $MOCK_LIBS $TEST_FLAGS
clox/out/check_memory
gcc -DTESTING -O0 -o clox/out/check_debug clox/__test__/check_debug.c clox/src/*.c $MOCK_LIBS $TEST_FLAGS
clox/out/check_debug
gcc -DTESTING -O0 -o clox/out/check_value clox/__test__/check_value.c clox/src/*.c $MOCK_LIBS $TEST_FLAGS
clox/out/check_value
gcc -DTESTING -O0 -o clox/out/check_vm clox/__test__/check_vm.c clox/src/*.c $MOCK_LIBS $TEST_FLAGS
clox/out/check_vm

lcov --capture --directory clox --output-file coverage_report/coverage.info -q 2>/dev/null
lcov --remove coverage_report/coverage.info "*/__test__/*" --output-file coverage_report/coverage_filtered.info -q 2>/dev/null
lcov --list coverage_report/coverage_filtered.info -q 2>/dev/null
genhtml coverage_report/coverage_filtered.info --output-directory coverage_report -q 2>/dev/null

