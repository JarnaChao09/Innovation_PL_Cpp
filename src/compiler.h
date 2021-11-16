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

    struct CodeGenerator : virtual Expr::Visitor, virtual Statement::Visitor {
        Chunk *current_chunk;
        Expr *current_expr_tree;
        int line;
        bool had_error;

        CodeGenerator(): current_chunk(nullptr), current_expr_tree(nullptr), line(0), had_error(false) {};

        explicit CodeGenerator(Chunk *chunk): current_chunk(chunk), current_expr_tree(nullptr), line(0), had_error(false) {};

        CodeGenerator(Chunk *current_chunk, Expr *tree): current_chunk(current_chunk), current_expr_tree(tree), line(0), had_error(false) {};

        CodeGenerator(Chunk *current_chunk, Expr *tree, int line): current_chunk(current_chunk), current_expr_tree(tree), line(line), had_error(false) {};

        Chunk *generate();

        Chunk *generate_with(Expr *tree);

        Chunk *generate_into(Chunk *chunk);

        Chunk *generate_into_with(Chunk *chunk, Expr *tree);

        void error(const std::string &message);

        void evaluate(Expr *expr);

        void visit_unary_expr(Expr::Unary *expr) override;

        void visit_binary_expr(Expr::Binary *expr) override;

        void visit_literal_expr(Expr::Literal *expr) override;

        void visit_grouping_expr(Expr::Grouping *expr) override;
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

        [[nodiscard]] Expr *parse_precedence(ParserPrecedence precedence, const std::string &message);

        [[nodiscard]] Expr *expression();

        [[nodiscard]] Expr *number();

        [[nodiscard]] Expr *grouping();

        [[nodiscard]] Expr *unary();

        [[nodiscard]] Expr *binary(Expr *left);

        [[nodiscard]] Expr *literal();
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
