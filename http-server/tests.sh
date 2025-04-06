#!/bin/bash

if [ ! -d "http-server/out" ]; then
  mkdir http-server/out
fi

TEST_PATH="http-server/__test__"
OUT_PATH="http-server/out"
SRC_PATH="http-server/src"

TEST_FLAGS="$(pkg-config --cflags --libs check) -fprofile-arcs -ftest-coverage -lgcov"

rm -f $OUT_PATH/*.gcda $OUT_PATH/*.gcno coverage.info coverage_filtered.info

echo -e "----  Unit tests  ----"
gcc -O0 -o $OUT_PATH/check_request $TEST_PATH/check_request.c $SRC_PATH/*.c $TEST_FLAGS && $OUT_PATH/check_request
gcc -O0 -o $OUT_PATH/check_serv_settings $TEST_PATH/check_serv_settings.c $SRC_PATH/*.c $TEST_FLAGS && $OUT_PATH/check_serv_settings

rm -f $OUT_PATH/*-mocks.*

lcov --capture --directory $OUT_PATH --output-file http-server/__test__/cov/http-server-coverage.info -q > /dev/null 2>&1
lcov --remove http-server/__test__/cov/http-server-coverage.info "*$TEST_PATH/*" --output-file http-server/__test__/cov/coverage_filtered.info -q > /dev/null 2>&1
genhtml http-server/__test__/cov/coverage_filtered.info --output-directory http-server/__test__/cov -q > /dev/null 2>&1




echo -e "\n----  Integration tests  ----"
gcc http-server/main.c http-server/src/*.c -lz -o http-server/out/server
http-server/out/server --directory /files &
SERVER_PID=$!

assert_eq() {
    local actual=$1
    local expected=$2
    local test_name=$3

    actual=$(echo "$actual" | tr -d '\r' | xargs)
    expected=$(echo "$expected" | xargs)

    if [ "$actual" = "$expected" ]; then
        echo -e "  \xE2\x9C\x94  $test_name"
    else
        echo -e "  \e[31m\u274C\e[0m  $test_name \n\texpected: $expected\n\tactual  : $actual"
    fi
}

CASES=(
    "curl -i -s http://localhost:4221" 
    "curl -i -s http://localhost:4221/echo/string"
    "curl -i -s http://localhost:4221/string"
    "curl -i -s http://localhost:4221/user-agent"
    "curl -i -s http://localhost:4221/files/file_4334 -H Content-Type: application/octet-stream --data file_content_4334"
)
EXPECTED=(
    "HTTP/1.1 200 OK" 
    "HTTP/1.1 200 OK Content-Type: text/plain Content-Length: 6 string"
    "HTTP/1.1 404 Not Found"
    "HTTP/1.1 200 OK Content-Type: text/plain Content-Length: 10 curl/8.5.0" 
    "HTTP/1.1 201 Created" 
)

for IDX in "${!CASES[@]}"; do
    CASE="${CASES[$IDX]}"
    RESPONSE=$($CASE)
    assert_eq "$RESPONSE" "${EXPECTED[$IDX]}" "$CASE"
done

kill $SERVER_PID