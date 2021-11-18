//
// Created by Jaran Chao on 11/2/21.
//

#ifndef INNOVATION_PL_AST_H
#define INNOVATION_PL_AST_H

#include "../token.h"
#include "../object.h"
#include <any>
#include <utility>
#include <vector>
#include <sstream>

namespace language {
    class Expr {
    public:
        enum class Type {
            Byte,
            Short,
            Int,
            Long,
            Float,
            Double,
            Char,
            String,
            Boolean,
            Null,
            Object,
        };

        class Assign;

        class Binary;

        class Call;

        class Get;

        class Literal;

        class Logical;

        class Set;

        class Grouping;

        class Super;

        class This;

        class Unary;

        class Variable;

        Expr() = default;

        class Visitor {
        public:
            virtual void visit_assign_expr(Assign *expr) {};

            virtual void visit_binary_expr(Binary *expr) {};

            virtual void visit_call_expr(Call *expr) {};

            virtual void visit_get_expr(Get *expr) {};

            virtual void visit_literal_expr(Literal *expr) {};

            virtual void visit_logical_expr(Logical *expr) {};

            virtual void visit_set_expr(Set *expr) {};

            virtual void visit_grouping_expr(Grouping *expr) {};

            virtual void visit_super_expr(Super *expr) {};

            virtual void visit_this_expr(This *expr) {};

            virtual void visit_unary_expr(Unary *expr) {};

            virtual void visit_variable_expr(Variable *expr) {};
        };

        virtual void accept(Visitor *visitor) {};

        virtual std::string to_string() { return ""; };

        virtual Type type() = 0;
    };

    class Expr::Assign : public virtual Expr {
    public:
        Assign(Token name, Expr *e_value) : value(e_value), name(name) {};

        void accept(Visitor *visitor) override {
            visitor->visit_assign_expr(this);
        }

        Type type() override {
            return value->type();
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Assign(" << std::string(this->name.start, this->name.length) << ", " << this->value->to_string() << ")";
            return out.str();
        }

        Token name;
        Expr *value;
    };

    class Expr::Binary : public virtual Expr {
    public:
        Binary(Expr *l, Token op, Expr *r) : left(l), op(op), right(r) {};

        void accept(Visitor *visitor) override {
            visitor->visit_binary_expr(this);
        }

        Type type() override {
            return left->type();
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Binary(" << this->left->to_string() << ", " << std::string(this->op.start, this->op.length) << ", " << this->right->to_string() << ")";
            return out.str();
        }

        Expr *left, *right;
        Token op;
    };

    class Expr::Call : public virtual Expr {
    public:
        Call(Expr *callee, Token paren, std::vector<Expr *> arguments, Type return_type) : callee(callee),
                                                                         arguments(std::move(arguments)),
                                                                         paren(paren),
                                                                         return_type(return_type) {};

        void accept(Visitor *visitor) override {
            visitor->visit_call_expr(this);
        }

        Type type() override {
            return return_type;
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Call(" << this->callee->to_string() << ", " << std::string(this->paren.start, this->paren.length) << ", ";
            for (auto i = 0; i < this->arguments.size(); i++) {
                out << this->arguments[i]->to_string() << (i == this->arguments.size() - 1 ? ")" : ", ");
            }
            return out.str();
        }

        Expr *callee;
        Token paren;
        std::vector<Expr *> arguments;
        Type return_type;
    };

    class Expr::Get : public virtual Expr {
    public:
        Get(Expr *object, Token name, Type get_type) : object(object), name(name), get_type(get_type) {};

        void accept(Visitor *visitor) override {
            visitor->visit_get_expr(this);
        }

        Type type() override {
            return get_type;
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Get(" << std::string(this->name.start, this->name.length) << ", " << this->object->to_string() << ")";
            return out.str();
        }

        Expr *object;
        Token name;
        Type get_type;
    };

    class Expr::Grouping : public virtual Expr {
    public:
        explicit Grouping(Expr *expression) : expression(expression) {};

        void accept(Visitor *visitor) override {
            visitor->visit_grouping_expr(this);
        }

        Type type() override {
            return expression->type();
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Grouping(" << this->expression->to_string() << ")";
            return out.str();
        }

        Expr *expression;
    };

    class Expr::Literal : public virtual Expr {
    public:
        Literal(language::value_t value, Type literal_type, int line) : value(value), literal_type(literal_type), line(line) {};

        void accept(Visitor *visitor) override {
            visitor->visit_literal_expr(this);
        }

        Type type() override {
            return literal_type;
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Literal(";
            language::Expr::Literal::Type _type = this->type();
            switch (_type) {
                case Expr::Literal::Type::Byte:
//                    out << std::any_cast<unsigned char>(this->value);
                    out << ", Byte";
                    break;
                case Expr::Literal::Type::Short:
//                    out << std::any_cast<short>(this->value);
                    out << ", Short";
                    break;
                case Expr::Literal::Type::Int:
//                    out << std::any_cast<int>(this->value);
                    out << ", Int";
                    break;
                case Expr::Literal::Type::Long:
//                    out << std::any_cast<long>(this->value);
                    out << ", Long";
                    break;
                case Expr::Literal::Type::Float:
//                    out << std::any_cast<float>(this->value);
                    out << ", Float";
                    break;
                case Expr::Literal::Type::Double:
                    out << this->value.as.double_value;
                    out << ", Double";
                    break;
                case Expr::Literal::Type::Char:
//                    out << std::any_cast<char>(this->value);
                    out << ", Char";
                    break;
                case Expr::Literal::Type::String:
                    out << reinterpret_cast<ObjString*>(value.as.obj)->chars;
                    out << ", String";
                    break;
                case Expr::Literal::Type::Boolean:
                    out << std::boolalpha << this->value.as.boolean_value;
                    out << ", Boolean";
                    break;
                case Expr::Literal::Type::Null:
                    out << "Null";
                    break;
                case Expr::Literal::Type::Object:
                    out << this->value.as.obj;
                    out << ", Object";
            }
            out << ")";
            return out.str();
        }

        language::value_t value;
        Type literal_type;
        int line;
    };

    class Expr::Logical : public virtual Expr {
    public:
        Logical(Expr *l, Token op, Expr *r) : left(l), right(r), op(op) {};

        void accept(Visitor *visitor) override {
            visitor->visit_logical_expr(this);
        }

        Type type() override {
            return Type::Boolean;
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Logical(" << this->left->to_string() << ", " << std::string(this->op.start, this->op.length) << ", " << this->right->to_string() << ")";
            return out.str();
        }

        Expr *left, *right;
        Token op;
    };

    class Expr::Set : public virtual Expr {
    public:
        Set(Expr *object, Token name, Expr *value) : object(object), value(value), name(name) {};

        void accept(Visitor *visitor) override {
            visitor->visit_set_expr(this);
        }

        Type type() override {
            return value->type();
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Get(" << std::string(this->name.start, this->name.length) << ", " << this->object->to_string() << this->value->to_string() << ")";
            return out.str();
        }

        Expr *object, *value;
        Token name;
    };

    class Expr::Super : public virtual Expr {
    public:
        Super(Token keyword, Token method) : keyword(keyword), method(method) {};

        void accept(Visitor *visitor) override {
            visitor->visit_super_expr(this);
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Super(" << std::string(this->keyword.start, this->keyword.length) << ", " << std::string(this->method.start, this->method.length) << ")";
            return out.str();
        }

        Token keyword, method;
    };

    class Expr::This : public virtual Expr {
    public:
        explicit This(Token keyword) : keyword(keyword) {};

        void accept(Visitor *visitor) override {
            visitor->visit_this_expr(this);
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Super(" << std::string(this->keyword.start, this->keyword.length) << ")";
            return out.str();
        }

        Token keyword;
    };

    class Expr::Unary : public virtual Expr {
    public:
        Unary(Token op, Expr *r) : right(r), op(op) {};

        void accept(Visitor *visitor) override {
            visitor->visit_unary_expr(this);
        }

        Type type() override {
            return right->type();
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Unary(" << std::string(this->op.start, this->op.length) << ", " << this->right->to_string() << ")";
            return out.str();
        }

        Token op;
        Expr *right;
    };

    class Expr::Variable : public virtual Expr {
    public:
        Variable(Token name, Type variable_type) : name(name), variable_type(variable_type) {};

        void accept(Visitor *visitor) override {
            visitor->visit_variable_expr(this);
        }

        Type type() override {
            return variable_type;
        }

        std::string to_string() override {
            std::stringstream out{};
            out << "Variable(" << std::string(this->name.start, this->name.length) << ")";
            return out.str();
        }

        Token name;
        Type variable_type;
    };

    class Statement {
    public:
        class Class;

        class Expression;

        class Function;

        class Block;

        class If;

        class Print;

        class Return;

        class Var;

        class While;

        class Visitor {
        public:
            void visit_class_stmt(Class *stmt) const {};

            void visit_expression_stmt(Expression *stmt) const {};

            void visit_function_stmt(Function *stmt) const {};

            void visit_block_stmt(Block *stmt) const {};

            void visit_if_stmt(If *stmt) const {};

            void visit_print_stmt(Print *stmt) const {};

            void visit_return_stmt(Return *stmt) const {};

            void visit_var_stmt(Var *stmt) const {};

            void visit_while_stmt(While *stmt) const {};
        };

        virtual void accept(Visitor *visitor) {}
    };

    class Statement::Block : public virtual Statement {
    public:
        explicit Block(std::vector<Statement *> statements) : statements(std::move(statements)) {};

        void accept(Visitor *visitor) override {
            visitor->visit_block_stmt(this);
        }

        std::vector<Statement *> statements;
    };

    class Statement::Expression : public virtual Statement {
    public:
        explicit Expression(Expr *expression) : expression(expression) {};

        void accept(Visitor *visitor) override {
            visitor->visit_expression_stmt(this);
        }

        language::Expr *expression;
    };

    class Statement::Class : public virtual Statement {
    public:
        Class(Token name, Expr::Variable *superclass, std::vector<Statement::Function *> methods) : superclass(
                superclass), methods(std::move(methods)), name(name) {};

        void accept(Visitor *visitor) override {
            visitor->visit_class_stmt(this);
        }

        Token name;
        Expr::Variable *superclass;
        std::vector<Statement::Function *> methods{};
    };

    class Statement::Function : public virtual Statement {
    public:
        Function(Token name, std::vector<Token> params, std::vector<Statement *> body) : params(std::move(params)),
                                                                                         body(std::move(body)),
                                                                                         name(name) {};

        void accept(Visitor *visitor) override {
            visitor->visit_function_stmt(this);
        }

        Token name;
        std::vector<Token> params;
        std::vector<Statement *> body;
    };

    class Statement::If : public virtual Statement {
    public:
        If(Expr *condition, Statement *then_branch, Statement *else_branch) : condition(condition),
                                                                              then_branch(then_branch),
                                                                              else_branch(else_branch) {};

        void accept(Visitor *visitor) override {
            visitor->visit_if_stmt(this);
        }

        Expr *condition;
        Statement *then_branch, *else_branch;
    };

    class Statement::Print : public virtual Statement {
    public:
        explicit Print(Expr *expression) : expression(expression) {};

        void accept(Visitor *visitor) override {
            visitor->visit_print_stmt(this);
        }

        Expr *expression;
    };

    class Statement::Return : public virtual Statement {
    public:
        Return(Token keyword, Expr *value) : value(value), keyword(keyword) {};

        void accept(Visitor *visitor) override {
            visitor->visit_return_stmt(this);
        }

        Token keyword;
        Expr *value;
    };

    class Statement::Var : public virtual Statement {
        Var(Token name, Expr *initializer) : initializer(initializer), name(name) {};

        void accept(Visitor *visitor) override {
            visitor->visit_var_stmt(this);
        }

        Token name;
        Expr *initializer;
    };

    class Statement::While : public virtual Statement {
        While(Expr *condition, Statement *body) : condition(condition), body(body) {}

        void accept(Visitor *visitor) override {
            visitor->visit_while_stmt(this);
        }

        Expr *condition;
        Statement *body;
    };
}

#endif //INNOVATION_PL_AST_H
