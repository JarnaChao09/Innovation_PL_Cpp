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

    auto instruction = static_cast<language::OpCode>(chunk->code[offset]);
    switch (instruction) {
        case language::OpCode::Constant:
            return constant_instruction("CONSTANT_OP", chunk, offset);
        case language::OpCode::Identity:
            return simple_instruction("IDENTITY_OP", offset);
        case language::OpCode::Negate:
            return simple_instruction("NEGATE_OP", offset);
        case language::OpCode::Not:
            return simple_instruction("NOT_OP", offset);
        case language::OpCode::Null:
            return simple_instruction("NULL_OP", offset);
        case language::OpCode::True:
            return simple_instruction("TRUE_OP", offset);
        case language::OpCode::False:
            return simple_instruction("FALSE_OP", offset);
        case language::OpCode::Ne:
            return simple_instruction("NOT_EQUAL_OP", offset);
        case language::OpCode::Eq:
            return simple_instruction("EQUAL_OP", offset);
        case language::OpCode::Gt:
            return simple_instruction("GREATER_THAN_OP", offset);
        case language::OpCode::Ge:
            return simple_instruction("GREATER_EQUAL_OP", offset);
        case language::OpCode::Lt:
            return simple_instruction("LESS_THAN_OP", offset);
        case language::OpCode::Le:
            return simple_instruction("LESS_EQUAL_OP", offset);
        case language::OpCode::Add:
            return simple_instruction("ADD_OP", offset);
        case language::OpCode::Subtract:
            return simple_instruction("SUBTRACT_OP", offset);
        case language::OpCode::Multiply:
            return simple_instruction("MULTIPLY_OP", offset);
        case language::OpCode::Divide:
            return simple_instruction("DIVIDE_OP", offset);
        case language::OpCode::Return:
            return simple_instruction("RETURN_OP", offset);
        default:
            printf("Unknown opcode %hhu\n", instruction);
            return offset + 1;
    }
}