//
// Created by Jaran Chao on 11/2/21.
//

#ifndef INNOVATION_PL_TOKEN_H
#define INNOVATION_PL_TOKEN_H

namespace language {
    enum class TokenType {
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
}

#endif //INNOVATION_PL_TOKEN_H
