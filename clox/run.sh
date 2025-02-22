#!/bin/bash

gcc clox/main.c clox/src/*.c -o clox/out/clox -DDEBUG_TRACE_EXECUTION=1 && ./clox/out/clox