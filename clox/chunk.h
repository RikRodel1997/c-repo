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
    OP_CONSTANT -> 0
    OP_RETURN -> 1
    ...
*/
typedef enum {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

/*
    The Chunk struct represents a sequence of bytecode instructions.
*/
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk* chunk);
void free_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
int add_constant(Chunk* chunk, Value value);

#endif
