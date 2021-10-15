//
// Created by Jaran Chao on 10/13/21.
//

#ifndef INNOVATION_PL_SCANNER_H
#define INNOVATION_PL_SCANNER_H

#include "common.h"

namespace language {
    enum TokenType {
        LeftParen, RightParen,
        LeftBrace, RightBrace,
        LeftBracket, RightBracket,

        Comma,
        Dot,
        Plus,
        Minus,
        Star,
        Slash,

        EOS,

        Bang, BangEq,
        BitComplement,
        EQ, EqEq,
        Gt, Ge,
        Le, Lt,
        BitAnd, And,
        BitOr, Or,
        XOR,

        Identifier, String, Number,

        True,
        False,
        If,
        Else,
        Return,
        While,
        For,
        Var,
        Val,
        Fun,
        Class,
        Null,
        This,
        Super,

        Error,
        EOF_T,
    };

    struct Token {
        TokenType type;
        const char *start;
        std::size_t length;
        int line;
    };

    struct Scanner {
        const char *start;
        const char *current;
        int line;

        explicit Scanner(const std::string &source) {
            const char *source_c = source.c_str();
            this->start = source_c;
            this->current = source_c;
            this->line = 1;
        }

        ~Scanner() {
            this->start = nullptr;
            this->current = nullptr;
        }

        Token scan_token();

        [[nodiscard]] Token make_token(TokenType token_type) const;

        [[nodiscard]] Token error_token(const std::string& message) const;

        [[nodiscard]] bool is_at_end() const;

        bool match(char expected);

        char advance();

        void skip_whitespace();

        [[nodiscard]] char peek() const;

        [[nodiscard]] char peek_next() const;

        Token string_literal();

        Token number_literal();

        Token identifier();

        [[nodiscard]] TokenType identifier_type() const;

        [[nodiscard]] TokenType check_keyword(int start_index, const std::string& rest, TokenType return_type) const;
    };
}
#endif //INNOVATION_PL_SCANNER_H
