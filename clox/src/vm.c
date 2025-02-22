#include "../vm.h"
#include "../common.h"
#include "../debug.h"
#include <stdio.h>

#define READ_BYTE()     (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                                                                  \
    do {                                                                                                               \
        double b = pop();                                                                                              \
        double a = pop();                                                                                              \
        push(a op b);                                                                                                  \
    } while (false)

VM vm;

static void reset_stack() { vm.stack_top = vm.stack; }

void init_vm() { reset_stack(); }

void free_vm() {}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

static InterpretResult run() {

    for (;;) {

#if DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            print_value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT:
            Value constant = READ_CONSTANT();
#if RUN_TEST
            print_value(constant);
            printf("\n");
#endif
            push(constant);
            break;
        case OP_ADD:
            BINARY_OP(+);
            break;
        case OP_SUBTRACT:
            BINARY_OP(-);
            break;
        case OP_MULTIPLY:
            BINARY_OP(*);
            break;
        case OP_DIVIDE:
            BINARY_OP(/);
            break;
        case OP_NEGATE:
            push(-pop());
            break;
        case OP_RETURN:
#if RUN_TEST
            print_value(pop());
            printf("\n");
#endif
            return INTERPRET_OK;
        }
    }
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP