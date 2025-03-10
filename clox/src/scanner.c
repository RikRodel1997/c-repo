#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../scanner.h"

bool is_at_end();
bool match(char expected);

Token make_token(TokenType type);
Token error_token(const char* msg);

char advance();
char peek();
char peek_next();
void skip_whitespace();

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
    skip_whitespace();
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
    case '!':
        if (match('=')) {
            return make_token(TOKEN_BANG_EQUAL);
        } else {
            return make_token(TOKEN_BANG);
        }
    case '=':
        if (match('=')) {
            return make_token(TOKEN_EQUAL_EQUAL);
        } else {
            return make_token(TOKEN_EQUAL);
        }
    case '<':
        if (match('=')) {
            return make_token(TOKEN_LESS_EQUAL);
        } else {
            return make_token(TOKEN_LESS);
        }
    case '>':
        if (match('=')) {
            return make_token(TOKEN_GREATER_EQUAL);
        } else {
            return make_token(TOKEN_GREATER);
        }
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

bool match(char expected) {
    if (is_at_end()) {
        return false;
    }
    if (*scanner.curr != expected) {
        return false;
    }
    scanner.curr++;
    return true;
}

void skip_whitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !is_at_end()) {
                    advance();
                }
            } else {
                return;
            }
            break;
        default:
            return;
        }
    }
}

char peek() { return *scanner.curr; }

char peek_next() {
    if (is_at_end()) {
        return '\0';
    }
    return scanner.curr[1];
}