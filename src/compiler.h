//
// Created by Jaran Chao on 10/13/21.
//

#ifndef INNOVATION_PL_COMPILER_H
#define INNOVATION_PL_COMPILER_H

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "scanner.h"

namespace language {
//    void compile(const std::string &source);

    bool compile(language::VM *vm, const std::string &source, language::Chunk &chunk);

    struct Parser {
        language::Scanner scanner;
        language::Token current;
        language::Token previous;

        bool had_error;
        bool panic_mode;

        explicit Parser(language::Scanner &scanner1)
                : scanner(scanner1),
                  current(), previous(), had_error(false), panic_mode(false) {}

        void error_at_current(const std::string &message);

        void error(const std::string &message);

        void error_at(language::Token &token, const std::string &message);

        void advance();

        void consume(language::TokenType type, const std::string &message);

        void emit_byte(std::uint8_t byte) const;

        void emit_bytes(std::uint8_t byte1, std::uint8_t byte2) const;

        void emit_return() const;

        void expression() const;
    };
}

#endif //INNOVATION_PL_COMPILER_H
