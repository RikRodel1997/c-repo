#ifndef MOCK_EXIT_H
#define MOCK_EXIT_H

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>


extern jmp_buf exit_buffer;
extern int exit_code;

#ifdef TESTING
#define exit(code) mock_exit(code)
#endif

void mock_exit(int code);
int setup_stdout(FILE* test_out);
void read_stdout(FILE* test_out, int og_stdout, char* buf);

#endif