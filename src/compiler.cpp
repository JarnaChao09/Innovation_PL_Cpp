//
// Created by Jaran Chao on 10/13/21.
//

#include "compiler.h"
#include "scanner.h"

//void language::compile(const std::string &source) {
//    auto scanner = language::Scanner(source);
//
//    int line = -1;
//
//    for (;;) {
//        language::Token token = scanner.scan_token();
//        if (token.line != line) {
//            printf("%4d ", token.line);
//            line = token.line;
//        } else {
//            std::cout << "   | ";
//        }
//
//        printf("%2d '%.*s'\n", token.type, static_cast<int>(token.length), token.start);
//
//        if (token.type == TokenType::EOF_T) {
//            break;
//        }
//    }
//}

language::Chunk *compiling_chunk;

static language::Chunk *current_chunk() {
    return compiling_chunk;
}

static void end_compiler(language::Parser &parser) {
    parser.emit_return();
}

void language::Parser::emit_byte(std::uint8_t byte) const {
    current_chunk()->write(byte, this->previous.line);
}

void language::Parser::emit_bytes(std::uint8_t byte1, std::uint8_t byte2) const {
    this->emit_byte(byte1);
    this->emit_byte(byte2);
}

void language::Parser::emit_return() const {
    this->emit_byte(static_cast<uint8_t>(language::OpCode::Return));
}

void language::Parser::advance() {
    this->previous = this->current;

    for (;;) {
        this->current = this->scanner.scan_token();

        if (this->current.type != language::TokenType::Error) break;

        this->error_at_current(this->current.start);
    }
}

void language::Parser::consume(language::TokenType type, const std::string &message) {
    if (this->current.type == type) {
        this->advance();
        return;
    }

    this->error_at_current(message);
}

void language::Parser::error_at_current(const std::string &message) {
    this->error_at(this->current, message);
}

void language::Parser::error(const std::string &message) {
    this->error_at(this->previous, message);
}

void language::Parser::error_at(language::Token &token, const std::string &message) {
    std::cerr << "[line " << std::setw(4) << token.line << "] Error";

    if (token.type == language::TokenType::EOF_T) {
        std::cerr << " at end";
    } else if (token.type == language::TokenType::Error) {

    } else {
        std::cerr << " at '" << std::string(token.start).substr(0, token.length) << "'";
    }

    std::cerr << ": " << message << "\n";
    this->had_error = true;
}

bool language::compile(language::VM *vm, const std::string &source, language::Chunk *chunk) {
    auto scanner = language::Scanner(source);

    compiling_chunk = chunk;

    auto parser = language::Parser(scanner);

    return false;
}

void language::Parser::expression() const {

}