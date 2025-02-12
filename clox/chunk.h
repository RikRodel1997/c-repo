/*
    A chunk represents the sequence of bytecode instructions that the VM will execute.
*/
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

/*
    The OpCode enum represents the different types of instructions that the VM can execute.
    The positions of the opcodes in the enum are important because the VM uses them to index into the disassembly table.
    OP_CONSTANT -> 00
    OP_RETURN -> 01
    ...
*/
typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN,
} OpCode;

typedef struct {
    int offset;
    int line;
} LineStart;

/*
    The Chunk struct represents a sequence of bytecode instructions.
*/
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
    int line_count;
    int line_capacity;
    LineStart* lines;
} Chunk;

void init_chunk(Chunk* chunk);
void free_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
int add_constant(Chunk* chunk, Value value);
int get_line(Chunk* chunk, int instruction);
void write_constant(Chunk* chunk, Value value, int line);

#endif
