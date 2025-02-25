#!/bin/bash

TST_PATH="lib/c/__test__"
LIBS="-lcheck -lm -lpthread -lrt -lsubunit"

gcc -o ./servers/check_tests_request ${TST_PATH}/check_request.c lib/c/src/request.c ${LIBS} && ./servers/check_tests_request
gcc -o ./servers/check_tests_serv_settings ${TST_PATH}/check_serv_settings.c lib/c/src/serv_settings.c ${LIBS} && ./servers/check_tests_serv_settings
