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
        }

        struct Param;
        struct Ident;
        struct FunctionDecl;
        struct CodeBlock;

        struct CodeBlock : ncompiler::Tree {
            explicit CodeBlock(const ncompiler::TreeKind &kind)
                : Tree(kind) {
            }

            [[nodiscard]] std::string toString() const override;
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

            const auto code_block = dynamic_cast<tree::CodeBlock*>(parser->parse(parser->rule("code_block")));
            // {code_block}

            return new tree::FunctionDecl(tree::kind::function_decl, name, std::move(params), return_type, code_block);
        }
    };

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

    struct CodeBlock : ncompiler::ParserRule {
        explicit CodeBlock(std::string &&name)
            : ParserRule(name) {
        }

        explicit CodeBlock(const std::string &name)
            : ParserRule(name) {
        }

        ncompiler::Tree * parse(ncompiler::Parser *parser) override {
            parser->accept({lexer_rules::token::l_brace});

            while (parser->probeNotAndRedoIf(true, {lexer_rules::token::r_brace})) {
                parser->next();
                std::cout << parser->tokens_stream->get().toString() << std::endl;
            }

            return new tree::CodeBlock(tree::kind::code_block);
        }
    };

    inline void init(ncompiler::Parser *parser) {
        parser->root_rule = new FunctionDecl("function_declaration");
        parser->addRule(new Ident("ident"));
        parser->addRule(new CodeBlock("code_block"));
        parser->addRule(new Param("param"));
        parser->addRule(parser->root_rule);
    }
}
