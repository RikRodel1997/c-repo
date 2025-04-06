#include "mocks.h"
#include "../chunk.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

jmp_buf exit_buffer;
int exit_code = 0;

void mock_exit(int code) {
    exit_code = code;
    longjmp(exit_buffer, 1);
}

int setup_stdout(FILE* test_out) {
    int og_stdout = dup(STDOUT_FILENO);
    dup2(fileno(test_out), STDOUT_FILENO);
    return og_stdout;
}

void read_stdout(FILE* test_out, int og_stdout, char* buf) {
    fflush(stdout);
    fseek(test_out, 0, SEEK_SET);

    size_t bytes = fread(buf, 1, 16384, test_out);
    buf[bytes] = '\0';

    dup2(og_stdout, STDOUT_FILENO);
    close(og_stdout);

    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == ' ')) {
        buf[--len] = '\0';
    }
}

int setup_stderr(FILE* test_out) {
    int og_stdout = dup(STDERR_FILENO);
    dup2(fileno(test_out), STDERR_FILENO);
    return og_stdout;
}

void read_stderr(FILE* test_out, int og_stdout, char* buf) {
    fflush(stderr);
    fseek(test_out, 0, SEEK_SET);

    size_t bytes = fread(buf, 1, 16384, test_out);
    buf[bytes] = '\0';

    dup2(og_stdout, STDERR_FILENO);
    close(og_stdout);

    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == ' ')) {
        buf[--len] = '\0';
    }
}