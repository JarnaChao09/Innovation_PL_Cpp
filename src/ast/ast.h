//
// Created by Jaran Chao on 11/2/21.
//

#ifndef INNOVATION_PL_AST_H
#define INNOVATION_PL_AST_H

#include "../token.h"
#include <any>
#include <utility>
#include <vector>

namespace language {
    class Expr {
        class Visitor {
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

        class Assign : virtual Expr {
        public:
            Assign(Token t_name, Expr *e_value) : value(e_value) {
                this->name = t_name;
            };

            void accept(Visitor visitor) override {
                visitor.visit_assign_expr(this);
            }

            Token name;
            Expr *value;
        };

        class Binary : virtual Expr {
        public:
            Binary(Expr *l, Token op, Expr *r) : left(l), op(op), right(r) {};

            void accept(Visitor visitor) override {
                visitor.visit_binary_expr(this);
            }

            Expr *left, right;
            Token op;
        };

        class Call : virtual Expr {
        public:
            Call(Expr *callee, Token paren, std::vector<Expr *> arguments) : callee(callee) {
                this->arguments = std::move(arguments);
                this->paren = paren;
            };

            void accept(Visitor visitor) override {
                visitor.visit_call_expr(this);
            }

            Expr *callee;
            Token paren;
            std::vector<Expr *> arguments;
        };

        class Get : virtual Expr {
        public:
            Get(Expr *object, Token name) : object(object) {
                this->name = name;
            };

            void accept(Visitor visitor) override {
                visitor.visit_get_expr(this);
            }

            Expr *object;
            Token name;
        };

        class Grouping : Expr {
        public:
            explicit Grouping(Expr *expression) : expression(expresseion) {};

            void accept(Visitor visitor) override {
                visitor.visit_grouping_expr(this);
            }

            Expr *expression;
        };

        class Literal : virtual Expr {
        public:
            explicit Literal(std::any value) : value(std::move(value)) {};

            void accept(Visitor visitor) override {
                visitor.visit_literal_expr(this);
            }

            std::any value;
        };

        class Logical : virtual Expr {
        public:
            Logical(Expr *l, Token op, Expr *r) : left(l), right(r) {
                this->op = op;
            }

            void accept(Visitor visitor) override {
                visitor.visit_logical_expr(this);
            }

            Expr *left, right;
            Token op;
        };

        class Set : virtual Expr {
        public:
            Set(Expr *object, Token name, Expr *value) : object(object), value(value) {
                this->name = name;
            }

            void accept(Visitor visitor) override {
                visitor.visit_set_expr(this);
            }

            Expr *object, value;
            Token name;
        };

        class Super : virtual Expr {
        public:
            Super(Token keyword, Token method) {
                this->keyword = keyword;
                this->method = method;
            }

            void accept(Visitor visitor) override {
                visitor.visit_super_expr(this);
            }

            Token keyword, method;
        };

        class This : virtual Expr {
        public:
            explicit This(Token keyword) {
                this->keyword = keyword;
            }

            void accept(Visitor visitor) override {
                visitor.visit_this_expr(this);
            }

            Token keyword;
        };

        class Unary : virtual Expr {
        public:
            Unary(Token op, Expr *r) : right(r) {
                this->op = op;
            }

            void accept(Visitor visitor) override {
                visitor.visit_unary_expr(this);
            }

            Token op;
            Expr *right;
        };

        class Variable : virtual Expr {
        public:
            explicit Variable(Token name) {
                this->name = name;
            }

            void accept(Visitor visitor) override {
                visitor.visit_variable_expr(this);
            }

            Token name;
        };

        virtual void accept(Visitor visitor) {};
    };

    class Statement {
        class Visitor {
            void visit_class_stmt(Class *stmt) {};

            void visit_expression_stmt(Expression *stmt) {};

            void visit_function_stmt(Function *stmt) {};

            void visit_block_stmt(Block *stmt) {};

            void visit_if_stmt(If *stmt) {};

            void visit_print_stmt(Print *stmt) {};

            void visit_return_stmt(Return *stmt) {};

            void visit_var_stmt(Var *stmt) {};

            void visit_while_stmt(While *stmt) {};
        };

        class Block : virtual Statement {
        public:
            explicit Block(std::vector<Statement *> statements) : statements(std::move(statements)) {};

            void accept(Visitor visitor) override {
                visitor.visit_block_stmt(this);
            }

            std::vector<Statement *> statements;
        };

        class Class : virtual Statement {
        public:
            Class(Token name, Expr::Variable *superclass, std::vector<Statement::Function *> methods) : superclass(
                    superclass), methods(std::move(methods)) {
                this->name = name;
            }

            void accept(Visitor visitor) override {
                visitor.visit_class_stmt(this);
            }

            Token name;
            Expr::Variable *superclass;
            std::vector<Statement::Function *> methods{};
        };

        class Function : virtual Statement {
        public:
            Function(Token name, std::vector<Token> params, std::vector<Statement *> body) : params(std::move(params)),
                                                                                             body(std::move(body)) {
                this->name = name;
            }

            void accept(Visitor visitor) override {
                visitor.visit_function_stmt(this);
            }

            Token name;
            std::vector<Token> params;
            std::vector<Statement *> body;
        };

        class If : virtual Statement {
        public:
            If(Expr *condition, Statement *then_branch, Statement *else_branch) : condition(condition),
                                                                                  then_branch(then_branch),
                                                                                  else_branch(else_branch) {};

            void accept(Visitor visitor) override {
                visitor.visit_if_stmt(this);
            }

            Expr *condition;
            Statement *then_branch, else_branch;
        };

        class Print : virtual Statement {
        public:
            explicit Print(Expr *expression) : expression(expression) {};

            void accept(Visitor visitor) override {
                visitor.visit_print_stmt(this);
            }

            Expr *expression;
        };

        class Return : virtual Statement {
        public:
            Return(Token keyword, Expr *value) : value(value) {
                this->keyword = keyword;
            }

            void accept(Visitor visitor) override {
                visitor.visit_return_stmt(this);
            }

            Token keyword;
            Expr *value;
        };

        class Variable : virtual Statement {
            Variable(Token name, Expr *initializer) : initializer(initializer) {
                this->name = name;
            }

            void accept(Visitor visitor) override {
                visitor.visit_var_stmt(this);
            }

            Token name;
            Expr *initializer;
        };

        class While : virtual Statement {
            While(Expr *condition, Statement *body) : condition(condition), body(body) {}

            void accept(Visitor visitor) override {
                visitor.visit_while_stmt(this);
            }

            Expr *condition;
            Statement *body;
        };

        virtual void accept(Visitor visitor) {}
    };
}

#endif //INNOVATION_PL_AST_H
