//
// Created by Jaran Chao on 11/8/21.
//

#include "common.h"
#include "token.h"

std::string language::stringify(TokenType type) {
    switch(type) {
        case TokenType::LeftParen:
            return "LeftParen";
        case TokenType::RightParen:
            return "RightParen";
        case TokenType::LeftBrace:
            return "LeftBrace";
        case TokenType::RightBrace:
            return "RightBrace";
        case TokenType::LeftBracket:
            return "LeftBracket";
        case TokenType::RightBracket:
            return "RightBracket";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Star:
            return "Star";
        case TokenType::Slash:
            return "Slash";
        case TokenType::EOS:
            return "EOS";
        case TokenType::Bang:
            return "Bang";
        case TokenType::BangEq:
            return "BangEq";
        case TokenType::BitComplement:
            return "BitComplement";
        case TokenType::EQ:
            return "Eq";
        case TokenType::EqEq:
            return "EqEq";
        case TokenType::Gt:
            return "Gt";
        case TokenType::Ge:
            return "Ge";
        case TokenType::Lt:
            return "Lt";
        case TokenType::Le:
            return "Le";
        case TokenType::BitAnd:
            return "BitAnd";
        case TokenType::And:
            return "And";
        case TokenType::BitOr:
            return "BitOr";
        case TokenType::Or:
            return "Or";
        case TokenType::XOR:
            return "XOR";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::String:
            return "String";
        case TokenType::Number:
            return "Number";
        case TokenType::True:
            return "True";
        case TokenType::False:
            return "False";
        case TokenType::If:
            return "If";
        case TokenType::Else:
            return "Else";
        case TokenType::Return:
            return "Return";
        case TokenType::While:
            return "While";
        case TokenType::For:
            return "For";
        case TokenType::Var:
            return "Var";
        case TokenType::Val:
            return "Val";
        case TokenType::Fun:
            return "Fun";
        case TokenType::Class:
            return "Class";
        case TokenType::Null:
            return "Null";
        case TokenType::This:
            return "This";
        case TokenType::Super:
            return "Super";
        case TokenType::Error:
            return "Error";
        case TokenType::EOF_T:
            return "EOF";
    }
}

std::string language::Token::to_string() {
    std::stringstream out{};
    out << "Token(" << stringify(this->type) << ", " << std::string(this->start, this->length) << ", " << this->line << ")";
    return out.str();
}