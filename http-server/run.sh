#!/bin/bash

if [ ! -d "http-server/out" ]; then
  mkdir http-server/out
fi

gcc http-server/main.c http-server/src/*.c -lz -o http-server/out/server && http-server/out/server
