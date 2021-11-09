//
// Created by Jaran Chao on 10/13/21.
//

#ifndef INNOVATION_PL_COMPILER_H
#define INNOVATION_PL_COMPILER_H

#include <functional>

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "scanner.h"
#include "ast/ast.h"

namespace language {
//    void compile(const std::string &source);

    struct Compiler {
        const std::string &current_source;
        std::vector<language::Statement *> statements;

        explicit Compiler(const std::string &current_source) : current_source(current_source), statements() {}

        language::Expr *compile(language::VM *vm, const std::string &source, bool &flag);
    };

//    bool compile(language::VM *vm, const std::string &source, language::Chunk *chunk);

    enum class ParserPrecedence {
        None,
        Assignment,  // =
        Or,          // ||
        And,         // &&
        Equality,    // == !=
        Comparison,  // < > <= >=
        Term,        // + -
        Factor,      // * /
        Unary,       // ! -
        Call,        // . ()
        Primary,
    };

    struct Parser {
        language::Scanner *scanner;
        language::Compiler *compiler;
        language::Token current;
        language::Token previous;

        bool had_error;
        bool panic_mode;

        explicit Parser(language::Scanner *scanner, language::Compiler *compiler)
                : scanner(scanner), compiler(compiler), current(), previous(), had_error(false), panic_mode(false) {}

        void error_at_current(const std::string &message);

        void error(const std::string &message);

        void error_at(language::Token &token, const std::string &message);

        void advance();

        void consume(language::TokenType type, const std::string &message);

        void emit_byte(std::uint8_t byte) const;

        void emit_bytes(std::uint8_t byte1, std::uint8_t byte2) const;

        void emit_return() const;

        [[nodiscard]] Expr *parse_precedence(ParserPrecedence precedence);

        [[nodiscard]] Expr *expression();

        [[nodiscard]] Expr *number();

        [[nodiscard]] Expr *grouping();

        [[nodiscard]] Expr *unary();

        [[nodiscard]] Expr *binary(Expr *left);
    };

    using ParseFn = const std::function<Expr *(Parser *)>;
    using ParseInfixFn = const std::function<Expr *(Parser *, Expr *)>;

    struct ParseRule {
        ParseFn prefix;
        ParseInfixFn infix;
        ParserPrecedence precedence;
    };
}

#endif //INNOVATION_PL_COMPILER_H
