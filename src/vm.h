//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_VM_H
#define INNOVATION_PL_VM_H

#include "common.h"
#include "chunk.h"
//#include "memory.h"

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
        value_t stack[STACK_MAX];
        value_t* stack_top;
        Obj *objects;

        VM(): chunk(), ip(), stack(), stack_top(), objects() {
            this->reset_stack();
            this->objects = nullptr;
        }

        ~VM() {
            this->chunk = nullptr;
            this->ip = nullptr;
            this->stack_top = nullptr;
            this->free_objects();
        }

//        InterpreterResult interpret(language::Chunk* chunk);

        InterpreterResult interpret(const std::string &source);

        InterpreterResult run();

        void reset_stack();

        void push(value_t value);

        value_t pop();

        value_t peek(int distance);

        void runtime_error(const char *format, ...);

        void concatenate();

        void free_objects();
    };
}
#endif //INNOVATION_PL_VM_H
