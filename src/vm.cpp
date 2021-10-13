//
// Created by Jaran Chao on 10/12/21.
//

#include "common.h"
#include "debug.h"
#include "vm.h"

language::InterpreterResult language::VM::interpret(language::Chunk *_chunk) {
    this->chunk = _chunk;
    this->ip = &this->chunk->code[0];
    return this->run();
}

language::InterpreterResult language::VM::run() {
#define READ_BYTE() (*this->ip++)
#define READ_CONSTANT() (this->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
      double b = this->pop(); \
      double a = this->pop(); \
      this->push(a op b); \
    } while (false)

    for (;;) {
        std::uint8_t instruction;
        switch(instruction = READ_BYTE()) {
#ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (value_type * slot = this->stack; slot < this->stack_top; slot++) {
                printf("[ ");
                language::print_value(*slot);
                printf(" ]");
            }
            printf("\n");
            debug::disassemble_instruction(this->chunk, (int)(std::size_t)(this->ip - this->chunk->code[0]));
#endif
            case language::OpCode::Constant: {
                language::value_type constant = READ_CONSTANT();
                this->push(constant);
                break;
            }
            case language::OpCode::Negate: {
                this->push(-this->pop());
                break;
            }
            case language::OpCode::Add:      BINARY_OP(+); break;
            case language::OpCode::Subtract: BINARY_OP(-); break;
            case language::OpCode::Multiply: BINARY_OP(*); break;
            case language::OpCode::Divide:   BINARY_OP(/); break;
            case language::OpCode::Return: {
                language::print_value(this->pop());
                printf("\n");
                return InterpreterResult::Ok;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

void language::VM::reset_stack() {
    this->stack_top = this->stack;
}

void language::VM::push(value_type value) {
    *this->stack_top++ = value;
}

language::value_type language::VM::pop() {
    return *--this->stack_top;
}