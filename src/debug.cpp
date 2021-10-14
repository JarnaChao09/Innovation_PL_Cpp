//
// Created by Jaran Chao on 10/12/21.
//

#include "debug.h"
#include "value.h"

void debug::disassemble_chunk(language::Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count();) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constant_instruction(const char *name, language::Chunk *chunk, int offset) {
    std::uint8_t constant = chunk->code[offset+1];

    printf("%-16s %4d '", name, constant);
    language::print_value(chunk->constants.values[constant]);
    printf("'\n");

    return offset + 2;
}

int debug::disassemble_instruction(language::Chunk *chunk, int offset) {
    printf("%04d ", offset);

    if (offset > 0 &&
        chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case language::OpCode::Constant:
            return constant_instruction("OP_CONSTANT", chunk, offset);
        case language::OpCode::Negate:
            return simple_instruction("OP_NEGATE", offset);
        case language::OpCode::Add:
            return simple_instruction("OP_ADD", offset);
        case language::OpCode::Subtract:
            return simple_instruction("OP_SUBTRACT", offset);
        case language::OpCode::Multiply:
            return simple_instruction("OP_MULTIPLY", offset);
        case language::OpCode::Divide:
            return simple_instruction("OP_DIVIDE", offset);
        case language::OpCode::Return:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}