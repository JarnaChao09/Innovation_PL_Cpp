//
// Created by Jaran Chao on 10/13/21.
//

#include "scanner.h"

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

language::Token language::Scanner::scan_token() {
//    std::cout << "[DEBUG SCAN TOKEN]: start = '" << *this->start << "' current = '" << *this->current << "'\n";
    this->skip_whitespace();

    this->start = this->current;

    if (this->is_at_end()) {
        return this->make_token(TokenType::EOF_T);
    }

    char c = this->advance();

    if (is_alpha(c)) {
        return this->identifier();
    }
    if (is_digit(c)) {
        return this->number_literal();
    }

    switch (c) {
        case '(':
            return this->make_token(TokenType::LeftParen);
        case ')':
            return this->make_token(TokenType::RightParen);
        case '{':
            return this->make_token(TokenType::LeftBrace);
        case '}':
            return this->make_token(TokenType::RightBrace);
        case '[':
            return this->make_token(TokenType::LeftBracket);
        case ']':
            return this->make_token(TokenType::RightBracket);
        case ',':
            return this->make_token(TokenType::Comma);
        case '.':
            return this->make_token(TokenType::Dot);
        case '+':
            return this->make_token(TokenType::Plus);
        case '-':
            return this->make_token(TokenType::Minus);
        case '*':
            return this->make_token(TokenType::Star);
        case '/':
            return this->make_token(TokenType::Slash);
        case ';':
            return this->make_token(TokenType::EOS);
        case '\n':
            return language::Token{
                    .type = TokenType::EOS,
                    .start = "\\n",
                    .length = 2,
                    .line = this->line++,
            };
        case '!':
            return this->make_token(this->match('=') ? TokenType::BangEq : TokenType::Bang);
        case '~':
            return this->make_token(TokenType::BitComplement);
        case '=':
            return this->make_token(this->match('=') ? TokenType::EqEq : TokenType::EQ);
        case '<':
            return this->make_token(this->match('=') ? TokenType::Le : TokenType::Lt);
        case '>':
            return this->make_token(this->match('=') ? TokenType::Ge : TokenType::Gt);
        case '&':
            return this->make_token(this->match('&') ? TokenType::And : TokenType::BitAnd);
        case '|':
            return this->make_token(this->match('|') ? TokenType::Or : TokenType::BitOr);
        case '^':
            return this->make_token(TokenType::XOR);
        case '"':
            return this->string_literal();
        default:
            return this->error_token("Unexpected Character.");
    }
}

char language::Scanner::advance() {
    this->current++;
    return this->current[-1];
}

bool language::Scanner::is_at_end() const {
    return *this->current == '\0';
}

bool language::Scanner::match(char expected) {
    if (this->is_at_end()) return false;
    if (*this->current != expected) return false;
    this->current++;
    return true;
}

void language::Scanner::skip_whitespace() {
    for (;;) {
        char c = this->peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                this->advance();
                break;
            case '/':
                if (this->peek_next() == '/') {
                    while (this->peek() != '\n' && !this->is_at_end()) {
                        this->advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

char language::Scanner::peek() const {
    return *this->current;
}

char language::Scanner::peek_next() const {
    return this->is_at_end() ? '\0' : this->current[1];
}

language::Token language::Scanner::string_literal() {
    while (this->peek() != '"' && !this->is_at_end()) {
        if (this->peek() == '\n') {
            this->line++;
        }

        this->advance();
    }

    if (this->is_at_end()) {
        return this->error_token("Unterminated String");
    }

    this->advance(); // advance past closing quote
    return this->make_token(TokenType::String);
}

language::Token language::Scanner::number_literal() {
    while (is_digit(this->peek())) this->advance();

    if (this->peek() == '.' && is_digit(this->peek_next())) {
        this->advance();

        while (is_digit(this->peek())) this->advance();
    }

    return this->make_token(TokenType::Number);
}

language::Token language::Scanner::identifier() {
    char next = this->peek();
    while (is_alpha(next) || is_digit(next)) {
        this->advance();
        next = this->peek();
    }

    return this->make_token(this->identifier_type());
}

language::TokenType language::Scanner::identifier_type() const {
    switch (this->start[0]) {
        case 't':
            if (this->current - this->start > 1) {
                switch(this->start[1]) {
                    case 'h':
                        return this->check_keyword(2,"is", TokenType::This);
                    case 'r':
                        return this->check_keyword(2, "ue", TokenType::True);
                }
            }
            break;
        case 'f':
            if (this->current - this->start > 1) {
                switch(this->start[1]) {
                    case 'a':
                        return this->check_keyword(2, "lse", TokenType::False);
                    case 'o':
                        return this->check_keyword(2, "r", TokenType::For);
                    case 'u':
                        return this->check_keyword(2, "n", TokenType::Fun);
                }
            }
            break;
        case 'i':
            return this->check_keyword(1, "f", TokenType::If);
        case 'e':
            return this->check_keyword(1, "lse", TokenType::Else);
        case 'r':
            return this->check_keyword(1, "eturn", TokenType::Return);
        case 'w':
            return this->check_keyword(1, "hile", TokenType::While);
        case 'c':
            return this->check_keyword(1, "lass", TokenType::Class);
        case 'n':
            return this->check_keyword(1, "ull", TokenType::Null);
        case 's':
            return this->check_keyword(1, "uper", TokenType::Super);
        case 'v':
            if(this->current - this->start > 1) {
                switch(this->start[1]) {
                    case 'a':
                        if (this->current - this->start > 2) {
                            switch(this->start[2]) {
                                case 'r':
                                    return TokenType::Var;
                                case 'l':
                                    return TokenType::Val;
                            }
                        }
                        break;
                }
            }
    }
    return TokenType::Identifier;
}

language::TokenType
language::Scanner::check_keyword(int start_index, const std::string &rest, language::TokenType return_type) const {
    std::size_t length = rest.length();
    if (this->current - this->start == start_index + length &&
        rest.compare(0, length, this->start + start_index, start_index, length)) {
        return return_type;
    }

    return TokenType::Identifier;
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