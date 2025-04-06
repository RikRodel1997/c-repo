#ifndef clox_compiler_h
#define clox_compiler_h

#include "chunk.h"
#include "scanner.h"
#include "value.h"

typedef struct {
    Token current;
    Token previous;
    bool had_error;
    bool in_panic;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,   // =
    PREC_OR,           // or
    PREC_AND,          // and
    PREC_EQUALITY,     // == !=
    PREC_COMPARISON,   // < > <= >=
    PREC_TERM,         // + -
    PREC_FACTOR,       // * /
    PREC_UNARY,        // ! -
    PREC_CALL,         // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

/**
 * Main functions
 */
bool compile(const char* source, Chunk* chunk);
void end_compiler();
Chunk* current_chunk();

/**
 * Util functions
 */
void advance_parser();
void consume(TokenType type, const char* message);
uint8_t make_constant(Value value);

/**
 * Error functions
 */
void error_at_current(const char* message);
void error(const char* message);
void error_at(Token* token, const char* message);

/**
 * Emit functions
 */
void emit_byte(uint8_t byte);
void emit_bytes(uint8_t byte1, uint8_t byte2);
void emit_return();
void emit_constant(Value value);

/**
 * Expression functions
 */
void expression();
void parse_number();
void grouping();
void unary();
void binary();
void parse_precedence(Precedence precedence);
ParseRule* get_rule(TokenType type);

#endif