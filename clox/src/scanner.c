#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../scanner.h"

bool is_at_end();
Token make_token(TokenType type);
Token error_token(const char* msg);
char advance();

typedef struct {
    const char* start;
    const char* curr;
    int line;
} Scanner;

Scanner scanner;

void init_scanner(const char* source) {
    scanner.start = source;
    scanner.curr = source;
    scanner.line = 1;
}

Token scan_token() {
    scanner.start = scanner.curr;
    if (is_at_end()) {
        return make_token(TOKEN_EOF);
    }

    char c = advance();
    switch (c) {
    case '(':
        return make_token(TOKEN_LEFT_PAREN);
    case ')':
        return make_token(TOKEN_RIGHT_PAREN);
    case '{':
        return make_token(TOKEN_LEFT_BRACE);
    case '}':
        return make_token(TOKEN_RIGHT_BRACE);
    case ';':
        return make_token(TOKEN_SEMICOLON);
    case ',':
        return make_token(TOKEN_COMMA);
    case '.':
        return make_token(TOKEN_DOT);
    case '-':
        return make_token(TOKEN_MINUS);
    case '+':
        return make_token(TOKEN_PLUS);
    case '/':
        return make_token(TOKEN_SLASH);
    case '*':
        return make_token(TOKEN_STAR);
    }

    return error_token("Unexpected character.");
}

bool is_at_end() { return *scanner.curr == '\0'; }

Token make_token(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.curr - scanner.start);
    token.line = scanner.line;
    return token;
}

Token error_token(const char* msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg;
    token.length = (int) strlen(msg);
    token.line = scanner.line;
    return token;
}

char advance() {
    scanner.curr++;
    return scanner.curr[-1];
}