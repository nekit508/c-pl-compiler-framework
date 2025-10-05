#pragma once
#include <cstdint>

#include "BasicLexer.h"
#include "compile/parser/Parser.h"

namespace example::parser_rules {
    inline uint64_t IDT = 1;

    namespace tree {
        namespace kind {
            inline ncompiler::TreeKind ident("ident", IDT++);
            inline ncompiler::TreeKind param("param", IDT++);
            inline ncompiler::TreeKind function_decl("function_decl", IDT++);
            inline ncompiler::TreeKind code_block("code_block", IDT++);
            inline ncompiler::TreeKind const_value("const_value", IDT++);
            inline ncompiler::TreeKind returnn("return", IDT++);

            enum const_value_type {
                number, boolean, string
            };

            [[nodiscard]] inline std::string to_string(const const_value_type &e) {
                switch (e) {
                    case boolean: return "boolean";
                    case number: return "number";
                    case string: return "string";
                    default: return "unknown";
                }
            }
        }

        struct Param;
        struct Ident;
        struct FunctionDecl;
        struct CodeBlock;
        struct ConstValue;
        struct ReturnStatement;

        struct Value : ncompiler::Tree {
            explicit Value(const ncompiler::TreeKind &kind)
                : Tree(kind) {
            }
        };

        struct Statement : ncompiler::Tree {
            explicit Statement(const ncompiler::TreeKind &kind)
                : Tree(kind) {
            }
        };

        struct ReturnStatement : Statement {
            Value *value;

            explicit ReturnStatement(const ncompiler::TreeKind &kind, Value *value)
                : Statement(kind), value(value) {
            }

            [[nodiscard]] std::string toString() const override;

            ~ReturnStatement() override;
        };

        /** Must obtain control over value. */
        struct ConstValue : Value {
            void *value;
            kind::const_value_type type;

            explicit ConstValue(const ncompiler::TreeKind &kind, void *value, const kind::const_value_type type)
                : Value(kind), value(value), type(type) {
            }

            ~ConstValue() override {
                delete value;
            }

            [[nodiscard]] std::string toString() const override;
        };

        struct CodeBlock : ncompiler::Tree {
            std::vector<Statement *> statements;

            explicit CodeBlock(const ncompiler::TreeKind &kind, std::vector<Statement *> &&statements)
                : Tree(kind), statements(statements) {
            }

            [[nodiscard]] std::string toString() const override;

            ~CodeBlock() override;
        };

        struct FunctionDecl : ncompiler::Tree {
            std::string name;

            std::vector<Param *> params;
            Ident *return_type;
            CodeBlock *code_block;

            explicit FunctionDecl(const ncompiler::TreeKind &kind, const std::string &name,
                                  std::vector<Param *> &&params, Ident *return_type, CodeBlock *code_block)
                : Tree(kind), name(name), params(params), return_type(return_type), code_block(code_block) {
            }

            ~FunctionDecl() override;

            [[nodiscard]] std::string toString() const override;

            void acceptChilds(ncompiler::TreeWalker *walker) override;
        };

        struct Ident : ncompiler::Tree {
            std::string name;

            explicit Ident(const ncompiler::TreeKind &kind, const std::string &name)
                : Tree(kind), name(name) {
            }

            [[nodiscard]] std::string toString() const override;
        };

        struct Param : ncompiler::Tree {
            Ident *type;
            std::string name;

            explicit Param(const ncompiler::TreeKind &kind, Ident *type, const std::string &name)
                : Tree(kind), type(type), name(name) {
            }

            [[nodiscard]] std::string toString() const override;

            void acceptChilds(ncompiler::TreeWalker *walker) override;

            ~Param() override;
        };
    }

    struct ReturnStatement : ncompiler::ParserRule {
        explicit ReturnStatement(std::string &&name)
            : ParserRule(name) {
        }

        explicit ReturnStatement(const std::string &name)
            : ParserRule(name) {
        }

        /** 'return' {value} */
        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            parser->accept({lexer_rules::token::returnn});

            return new tree::ReturnStatement(tree::kind::returnn,
                                             dynamic_cast<tree::Value *>(parser->parse(parser->rule("value"))));
        }
    };

    struct Value : ncompiler::ParserRule {
        explicit Value(std::string &&name)
            : ParserRule(name) {
        }

        explicit Value(const std::string &name)
            : ParserRule(name) {
        }

        /** {const_value} */
        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            return parser->parseAny({parser->rule("const_value")});
        }
    };

    struct ConstValue : ncompiler::ParserRule {
        explicit ConstValue(std::string &&name)
            : ParserRule(name) {
        }

        explicit ConstValue(const std::string &name)
            : ParserRule(name) {
        }

        /** <value> */
        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            const auto token = parser->accept({
                lexer_rules::token::string,
                lexer_rules::token::number,
                lexer_rules::token::truee,
                lexer_rules::token::falsee
            });

            void *value = nullptr;
            tree::kind::const_value_type type;

            if (token == lexer_rules::token::string) {
                value = new std::string(token.literal);
                type = tree::kind::string;
            } else if (token == lexer_rules::token::number) {
                value = new uint64_t;
                type = tree::kind::number;
            } else if (token == lexer_rules::token::falsee) {
                value = new bool(false);
                type = tree::kind::boolean;
            } else if (token == lexer_rules::token::truee) {
                value = new bool(true);
                type = tree::kind::boolean;
            }

            if (value == nullptr)
                throw ncompiler::ParseFail("Unknown const value kind.", token.pos);

            return new tree::ConstValue(tree::kind::const_value, value, type);
        }
    };

    struct Statement : ncompiler::ParserRule {
        explicit Statement(std::string &&name)
            : ParserRule(name) {
        }

        explicit Statement(const std::string &name)
            : ParserRule(name) {
        }

        /** {return_statement} ';' */
        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            return parser->parseAny({parser->rule("return_statement")});
        }
    };

    struct FunctionDecl : ncompiler::ParserRule {
        explicit FunctionDecl(std::string &&name)
            : ParserRule(name) {
        }

        explicit FunctionDecl(const std::string &name)
            : ParserRule(name) {
        }

        /** 'dev' <name> '(' ({param} (',' {param})*)? ')' {code_block} */
        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            std::vector<tree::Param *> params = {};

            parser->accept({lexer_rules::token::def});
            // 'def'

            const std::string name = parser->accept({lexer_rules::token::ident}).literal;
            // <name>

            parser->accept({lexer_rules::token::l_paren});
            // '('
            if (parser->probeNotAndRedoIf(true, {lexer_rules::token::r_paren})) {
                do
                    params.emplace_back(dynamic_cast<tree::Param *>(parser->parse(parser->rule("param"))));
                while (parser->probeAndRedoIf(false, {lexer_rules::token::comma}));
                parser->accept({lexer_rules::token::r_paren});
            }
            // ({param} (, {param})*)* ')'

            parser->accept({lexer_rules::token::minus});
            parser->accept({lexer_rules::token::gr});
            // ->

            const auto return_type = dynamic_cast<tree::Ident *>(parser->parse(parser->rule("ident")));
            // {ident}

            const auto code_block = dynamic_cast<tree::CodeBlock *>(parser->parse(parser->rule("code_block")));
            // {code_block}

            return new tree::FunctionDecl(tree::kind::function_decl, name, std::move(params), return_type, code_block);
        }
    };

    /** {ident} <name> */
    struct Param : ncompiler::ParserRule {
        explicit Param(std::string &&name)
            : ParserRule(name) {
        }

        explicit Param(const std::string &name)
            : ParserRule(name) {
        }

        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            const auto type = dynamic_cast<tree::Ident *>(parser->parse(parser->rule("ident")));
            // {ident}
            return new tree::Param(tree::kind::param, type, parser->accept({lexer_rules::token::ident}).literal);
            // <name>
        }
    };

    /** <name> */
    struct Ident : ncompiler::ParserRule {
        explicit Ident(std::string &&name)
            : ParserRule(name) {
        }

        explicit Ident(const std::string &name)
            : ParserRule(name) {
        }

        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            return new tree::Ident(tree::kind::ident, parser->accept({lexer_rules::token::ident}).literal);
            // <name>
        }
    };

    /** '{' {statement}* '}' */
    struct CodeBlock : ncompiler::ParserRule {
        explicit CodeBlock(std::string &&name)
            : ParserRule(name) {
        }

        explicit CodeBlock(const std::string &name)
            : ParserRule(name) {
        }

        ncompiler::Tree *parse(ncompiler::Parser *parser) override {
            parser->accept({lexer_rules::token::l_brace});
            std::vector<tree::Statement*> statements;

            while (parser->probeNotAndRedoIf(true, {lexer_rules::token::r_brace})) {
                statements.emplace_back(dynamic_cast<tree::Statement *>(parser->parse(parser->rule("statement"))));
                parser->accept({lexer_rules::token::semicolon});
            }

            return new tree::CodeBlock(tree::kind::code_block, std::move(statements));
        }
    };

    inline void init(ncompiler::Parser *parser) {
        parser->root_rule = new FunctionDecl("function_declaration");
        parser->addRule(new Ident("ident"));
        parser->addRule(new CodeBlock("code_block"));
        parser->addRule(new Param("param"));
        parser->addRule(new Value("value"));
        parser->addRule(new ConstValue("const_value"));
        parser->addRule(new Statement("statement"));
        parser->addRule(new ReturnStatement("return_statement"));
        parser->addRule(parser->root_rule);
    }
}
