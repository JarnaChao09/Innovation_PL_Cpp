//
// Created by Jaran Chao on 10/13/21.
//

#include "scanner.h"

language::Token language::Scanner::scan_token() {
    this->start = this->current;

    if (this->is_at_end()) return this->make_token(TokenType::EOF_T);

    char c = this->advance();

    switch(c) {
        case '(': return this->make_token(TokenType::LeftParen);
        case ')': return this->make_token(TokenType::RightParen);
        case '{': return this->make_token(TokenType::LeftBrace);
        case '}': return this->make_token(TokenType::RightBrace);
        case '[': return this->make_token(TokenType::LeftBracket);
        case ']': return this->make_token(TokenType::RightBracket);
        case ',': return this->make_token(TokenType::Comma);
        case '.': return this->make_token(TokenType::Dot);
        case '+': return this->make_token(TokenType::Plus);
        case '-': return this->make_token(TokenType::Minus);
        case '*': return this->make_token(TokenType::Star);
        case '/': return this->make_token(TokenType::Slash);
        case ';': return this->make_token(TokenType::Semicolon);
    }

    return this->error_token("Unexpected Character.");
}

char language::Scanner::advance() {
    return *this->current++;
}

bool language::Scanner::is_at_end() const {
    return *this->current == '\0';
}

language::Token language::Scanner::make_token(language::TokenType token_type) const {
    return language::Token{
            .type = token_type,
            .start = this->start,
            .length = static_cast<std::size_t>(this->current - this->start),
            .line = this->line,
    };
}

language::Token language::Scanner::error_token(const std::string &message) const {
    return language::Token{
            .type = TokenType::Error,
            .start = message.c_str(),
            .length = message.size(),
            .line = this->line,
    };
}