#!/bin/bash

TEST_PATH="http-server/tests"
OUT_PATH="http-server/out"
SRC_PATH="http-server/src"

LIBS="-lcheck -lm -lpthread -lrt -lsubunit"

gcc -o $OUT_PATH/check_request $TEST_PATH/check_request.c $SRC_PATH/request.c $LIBS && $OUT_PATH/check_request
gcc -o $OUT_PATH/check_serv_settings $TEST_PATH/check_serv_settings.c $SRC_PATH/serv_settings.c $LIBS && $OUT_PATH/check_serv_settings
