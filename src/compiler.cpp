//
// Created by Jaran Chao on 10/13/21.
//
#include <unordered_map>

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
        this->current = this->scanner->scan_token();

//        std::cout << "[DEBUG ADVANCE]: current_token = '" << std::string(this->current.start, this->current.length) << "' " << language::stringify(this->current.type) << "\n";

        if (this->current.type != language::TokenType::Error) break;

        this->error_at_current(this->current.start);
    }
}

void language::Parser::consume(language::TokenType type, const std::string &message) {
    if (this->current.type == type) {
//        std::cout << "[DEBUG CONSUME]: " << std::string(this->current.start, this->current.length) << "\n";
//        std::cout << "                 previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
        this->advance();
//        std::cout << "                 previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
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
    if (this->panic_mode) return;
    this->panic_mode = true;
    std::cout << "[line " << std::setw(4) << token.line << "] Error";

    if (token.type == language::TokenType::EOF_T) {
        std::cout << " at end";
    } else if (token.type == language::TokenType::Error) {

    } else {
        std::cout << " at '" << std::string(token.start).substr(0, token.length) << "'";
    }

    std::cout << ": " << message << "\n";
    this->had_error = true;
}

language::Expr *language::Compiler::compile(language::VM *vm, const std::string &source, bool &flag) {
    auto scanner = language::Scanner(source);

    auto parser = language::Parser(&scanner, this);

    parser.advance();

    Expr *expr = parser.expression();

    parser.consume(TokenType::EOF_T, "Expect end of expression.");

    flag = parser.had_error;

    return expr;
}

static std::unordered_map<language::TokenType, language::ParseRule> &get_rules() {
    static language::ParseFn unary = [](language::Parser *parser) { return parser->unary(); };
    static language::ParseFn literal = [](language::Parser *parser) { return parser->literal(); };
    static language::ParseInfixFn binary = [](language::Parser *parser, language::Expr *left) { return parser->binary(left); };
    static std::unordered_map<language::TokenType, language::ParseRule> rules = {
            {language::TokenType::LeftParen    , language::ParseRule{[](language::Parser *parser) { return parser->grouping(); }, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::RightParen   , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::LeftBrace    , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::RightBrace   , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Comma        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Dot          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Plus         , language::ParseRule{unary, binary, language::ParserPrecedence::Term}},
            {language::TokenType::Minus        , language::ParseRule{unary, binary, language::ParserPrecedence::Term}},
            {language::TokenType::Star         , language::ParseRule{nullptr, binary, language::ParserPrecedence::Factor}},
            {language::TokenType::Slash        , language::ParseRule{nullptr, binary, language::ParserPrecedence::Factor}},
            {language::TokenType::EOS          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Bang         , language::ParseRule{unary, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::BangEq       , language::ParseRule{nullptr, binary, language::ParserPrecedence::Equality}},
            {language::TokenType::BitComplement, language::ParseRule{unary, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::EQ           , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::EqEq         , language::ParseRule{nullptr, binary, language::ParserPrecedence::Equality}},
            {language::TokenType::Gt           , language::ParseRule{nullptr, binary, language::ParserPrecedence::Comparison}},
            {language::TokenType::Ge           , language::ParseRule{nullptr, binary, language::ParserPrecedence::Comparison}},
            {language::TokenType::Lt           , language::ParseRule{nullptr, binary, language::ParserPrecedence::Comparison}},
            {language::TokenType::Le           , language::ParseRule{nullptr, binary, language::ParserPrecedence::Comparison}},
            {language::TokenType::BitAnd       , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::And          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::BitOr        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Or           , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::XOR          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Identifier   , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::String       , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Number       , language::ParseRule{[](language::Parser *parser) { return parser->number(); }, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::True         , language::ParseRule{literal, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::False        , language::ParseRule{literal, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::If           , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Else         , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Return       , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::While        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::For          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Var          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Val          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Fun          , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Class        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Null         , language::ParseRule{literal, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::This         , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Super        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::Error        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
            {language::TokenType::EOF_T        , language::ParseRule{nullptr, nullptr, language::ParserPrecedence::None}},
    };

    return rules;
}

language::ParseRule *get_rule(language::TokenType token_type) {
    return &get_rules()[token_type];
}

language::Expr *language::Parser::parse_precedence(language::ParserPrecedence precedence, const std::string &message) {
//    std::cout << "[DEBUG PARSE PRECEDENCE]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
    this->advance();
    language::ParseFn prefix_rule = get_rule(this->previous.type)->prefix;
    if (prefix_rule == nullptr) {
        this->error(message);
        return nullptr;
    }
    Expr *left = prefix_rule(this);

    Expr *final = left;

    while (precedence <= get_rule(this->current.type)->precedence) {
//        std::cout << "[DEBUG IN PRECEDENCE LOOP]\n";
        this->advance();
        language::ParseInfixFn infix_rule = get_rule(this->previous.type)->infix;
        final = infix_rule(this, final);
    }

    return final;
}

language::Expr *language::Parser::expression() {
//    std::cout << "[DEBUG EXPRESSION]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
    return this->parse_precedence(ParserPrecedence::Assignment, "Expect expression");
}

language::Expr *language::Parser::number() {
//    std::cout << "[DEBUG NUMBER]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
    double value = std::strtod(this->previous.start, nullptr);
    language::Expr *literal = new language::Expr::Literal(language::value_t(ValueType::Double, value), Expr::Literal::Type::Double, this->previous.line);
//    language::Statement *statement = new language::Statement::Expression(literal);
    return literal;
}

language::Expr *language::Parser::grouping() {
//    std::cout << "[DEBUG GROUPING]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
    language::Expr *expression = this->expression();
    this->consume(language::TokenType::RightParen, "Expected ')' after expression");
    return new language::Expr::Grouping(expression);
}

language::Expr *language::Parser::unary() {
//    std::cout << "[DEBUG UNARY]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";;
    language::Token prev_operator = this->previous;
    language::TokenType operator_type = this->previous.type;

    language::Expr *expression = this->parse_precedence(ParserPrecedence::Unary, "Expect expression after unary op '" + std::string(prev_operator.start, prev_operator.length) + "'");

    switch (operator_type) {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Bang:
        case TokenType::BitComplement:
            return new language::Expr::Unary(prev_operator, expression);
        default:
            return nullptr; // unreachable
    }
}

language::Expr *language::Parser::binary(language::Expr* left) {
//    std::cout << "[DEBUG BINARY]: previous = '" << std::string(this->previous.start, this->previous.length) << "', current = '" << std::string(this->current.start, this->current.length) << "'\n";
    language::Token prev_operator = this->previous;
    language::TokenType operator_type = prev_operator.type;
    language::ParseRule *rule = get_rule(operator_type);
    Expr* right = this->parse_precedence(static_cast<ParserPrecedence>(static_cast<int>(rule->precedence) + 1), "Expect expression after binary op '" + std::string(prev_operator.start, prev_operator.length) + "'");

    switch (operator_type) {
        case language::TokenType::BangEq:
        case language::TokenType::EqEq:
        case language::TokenType::Gt:
        case language::TokenType::Ge:
        case language::TokenType::Lt:
        case language::TokenType::Le:
        case language::TokenType::Plus:
        case language::TokenType::Minus:
        case language::TokenType::Star:
        case language::TokenType::Slash:
            return new language::Expr::Binary(left, prev_operator, right);
        default:
            return nullptr; // unreachable
    }
}

language::Expr *language::Parser::literal() {
    switch (this->previous.type) {
        case TokenType::False:
            return new language::Expr::Literal(language::value_t(ValueType::Boolean, false), Expr::Literal::Type::Boolean, this->previous.line);
        case TokenType::True:
            return new language::Expr::Literal(language::value_t(ValueType::Boolean, true), Expr::Literal::Type::Boolean, this->previous.line);
        case TokenType::Null:
            return new language::Expr::Literal(language::value_t(language::ValueType::Null, 0.0), Expr::Literal::Type::Null, this->previous.line);
        default:
            return nullptr; // unreachable
    }
}

language::Chunk *language::CodeGenerator::generate() {
    this->evaluate(this->current_expr_tree);
    this->current_chunk->write(static_cast<std::uint8_t>(language::OpCode::Return), this->line);
    return this->current_chunk;
}

language::Chunk *language::CodeGenerator::generate_with(Expr *tree) {
    this->current_expr_tree = tree;
    return this->generate();
}

language::Chunk *language::CodeGenerator::generate_into(language::Chunk *chunk) {
    this->current_chunk = chunk;
    return this->generate();
}

language::Chunk *language::CodeGenerator::generate_into_with(Chunk *chunk, Expr *tree) {
    this->current_chunk = chunk;
    this->current_expr_tree = tree;
    return this->generate();
}

void language::CodeGenerator::error(const std::string &message) {
    std::cout << "[line " << std::setw(4) << this->line << "] Error: " << message << "\n";
    this->had_error = true;
}

void language::CodeGenerator::evaluate(Expr *expr) {
    if (this->had_error) return;

    expr->accept(this);
}

void language::CodeGenerator::visit_unary_expr(Expr::Unary *expr) {
    if (this->had_error) return;

    this->evaluate(expr->right);

    language::OpCode opcode;

    switch (expr->op.type) {
        case TokenType::Plus:
            if (expr->type() != Expr::Type::Double) {
                this->error("Value after '+' must be a number");
                return;
            }
            opcode = language::OpCode::Identity;
            break;
        case TokenType::Minus:
            if (expr->type() != Expr::Type::Double) {
                this->error("Value after '-' must be a number");
                return;
            }
            opcode = language::OpCode::Negate;
            break;
        case TokenType::Bang:
            if (expr->type() != Expr::Type::Boolean && expr->type() != Expr::Type::Null) {
                this->error("Value after '!' must be a boolean (or null value)");
                return;
            }
            opcode = language::OpCode::Not;
            break;
        case TokenType::BitComplement:
            return; // not implemented
        default:
            return; // unreachable
    }

    this->current_chunk->write(static_cast<std::uint8_t>(opcode), this->line = expr->op.line);
}

void language::CodeGenerator::visit_binary_expr(Expr::Binary *expr) {
    if (this->had_error) return;

    this->evaluate(expr->left);
    this->evaluate(expr->right);

    language::OpCode opcode;

    auto left_type  = expr->left->type();
    auto right_type = expr->right->type();

    switch (expr->op.type) {
        case language::TokenType::BangEq:
            opcode = language::OpCode::Ne;
            break;
        case language::TokenType::EqEq:
            opcode = language::OpCode::Eq;
            break;
        case language::TokenType::Gt:
            opcode = language::OpCode::Gt;
            break;
        case language::TokenType::Ge:
            opcode = language::OpCode::Ge;
            break;
        case language::TokenType::Lt:
            opcode = language::OpCode::Lt;
            break;
        case language::TokenType::Le:
            opcode = language::OpCode::Le;
            break;
        case language::TokenType::Plus:
            opcode = language::OpCode::Add;
            break;
        case language::TokenType::Minus:
            opcode = language::OpCode::Subtract;
            break;
        case language::TokenType::Star:
            opcode = language::OpCode::Multiply;
            break;
        case language::TokenType::Slash:
            opcode = language::OpCode::Divide;
            break;
        default:
            return; // unreachable
    }

    switch (opcode) {
        case language::OpCode::Gt:
        case language::OpCode::Ge:
        case language::OpCode::Lt:
        case language::OpCode::Le:
        case language::OpCode::Add:
        case language::OpCode::Subtract:
        case language::OpCode::Multiply:
        case language::OpCode::Divide:
            if (left_type != Expr::Type::Double || right_type != Expr::Type::Double) {
                this->error("Both arguments of a binary expression '" + std::string(expr->op.start, expr->op.length) + "' must be numbers");
                return;
            }
            break;
        default:
            break;
    }

    this->current_chunk->write(static_cast<std::uint8_t>(opcode), this->line = expr->op.line);
}

void language::CodeGenerator::visit_literal_expr(Expr::Literal *expr) {
    if (this->had_error) return;

    switch (expr->type()) {
        case language::Expr::Literal::Type::Byte:
        case language::Expr::Literal::Type::Short:
        case language::Expr::Literal::Type::Int:
        case language::Expr::Literal::Type::Long:
        case language::Expr::Literal::Type::Float:
        case language::Expr::Literal::Type::Double:
            this->current_chunk->write(static_cast<std::uint8_t>(language::OpCode::Constant), this->line = expr->line);
            this->current_chunk->write(this->current_chunk->add_constant(expr->value), this->line = expr->line);
            break;
        case language::Expr::Literal::Type::Char:
        case language::Expr::Literal::Type::String:
            return; // not implemented
        case language::Expr::Literal::Type::Boolean:
            this->current_chunk->write(static_cast<std::uint8_t>(expr->value.as.boolean_value ? language::OpCode::True : language::OpCode::False), this->line = expr->line);
            break;
        case language::Expr::Literal::Type::Null:
            this->current_chunk->write(static_cast<std::uint8_t>(language::OpCode::Null), this->line = expr->line);
            break;
    }
}

void language::CodeGenerator::visit_grouping_expr(Expr::Grouping *expr) {
    if (this->had_error) return;

    this->evaluate(expr->expression);
}