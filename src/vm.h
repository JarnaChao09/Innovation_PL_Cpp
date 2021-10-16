//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_VM_H
#define INNOVATION_PL_VM_H

#include "common.h"
#include "chunk.h"

#define STACK_MAX 256

namespace language {
    enum class InterpreterResult {
        Ok,
        CompileError,
        RuntimeError,
    };

    struct VM {
        language::Chunk* chunk;
        std::uint8_t* ip;
        value_type stack[STACK_MAX];
        value_type* stack_top;

        VM(): chunk(), ip(), stack(), stack_top() {
            this->reset_stack();
        }

        ~VM() {
            this->chunk = nullptr;
            this->ip = nullptr;
            this->stack_top = nullptr;
        }

//        InterpreterResult interpret(language::Chunk* chunk);

        InterpreterResult interpret(const std::string &source);

        InterpreterResult run();

        void reset_stack();

        void push(value_type value);

        value_type pop();
    };
}
#endif //INNOVATION_PL_VM_H
