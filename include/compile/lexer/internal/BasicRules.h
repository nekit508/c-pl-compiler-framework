#pragma once

#include "compile/lexer/Lexer.h"
#include "compile/lexer/LexerRule.h"

namespace ncompiler::lexer_rules {
    inline auto ident_t = new TokenKind("ident");
    inline auto colon_t = new TokenKind("colon");
    inline auto l_brace_t = new TokenKind("lBrace");
    inline auto r_brace_t = new TokenKind("rBrace");
    inline auto l_paren_t = new TokenKind("lParen");
    inline auto r_paren_t = new TokenKind("rParen");
    inline auto l_bracket_t = new TokenKind("lBracket");
    inline auto r_bracket_t = new TokenKind("rBracket");

    inline LexerRule *ident, *one_symbol, *ignore_whitespaces;

    inline std::vector whitespaces_l = {
        ' ', '\t', '\n', '\r'
    };

    inline std::vector ident_start_l = {
        'A', 'B', 'C', 'D', 'E',
        'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y',
        'Z',
        'a', 'b', 'c', 'd', 'e',
        'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y',
        'z',
        '$', '_'
    };
    inline std::vector ident_body_l = {
        'A', 'B', 'C', 'D', 'E',
        'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y',
        'Z',
        'a', 'b', 'c', 'd', 'e',
        'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y',
        'z',
        '$', '_',
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9'
    };

    inline std::vector one_symbol_l = {
        ':',
        '{',
        '}',
        '(',
        ')',
        '[',
        ']'
    };

    inline std::vector one_symbol_t = {
        colon_t,
        l_brace_t,
        r_brace_t,
        l_paren_t,
        r_paren_t,
        l_bracket_t,
        r_bracket_t
    };

    struct IgnoreWhitespaces : LexerRule {
        explicit IgnoreWhitespaces(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        void parse(Lexer *lexer) noexcept(false) override {
            while (in(lexer->getNextChar(), whitespaces_l));
        }

        bool canParse(Lexer *lexer) noexcept(false) override {
            return in(lexer->getChar(), whitespaces_l);
        }
    };

    struct Ident : LexerRule {
        explicit Ident(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        bool canParse(Lexer *lexer) noexcept(false) override {
            return in(lexer->getChar(), ident_start_l);
        }

        void parse(Lexer *lexer) noexcept(false) override {
            do {
                lexer->putCurChar();
            } while (in(lexer->getNextChar(), ident_body_l));
            lexer->tokenParsed(ident_t);
        }
    };

    struct OneSymbol : LexerRule {
        explicit OneSymbol(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        bool canParse(Lexer *lexer) noexcept(false) override {
            return in(lexer->getChar(), one_symbol_l);
        }

        void parse(Lexer *lexer) noexcept(false) override {
            const char c = lexer->getChar();
            for(auto i = 0; i < one_symbol_l.size(); ++i) {
                if (one_symbol_l[i] == c) {
                    lexer->putChar(c);
                    lexer->tokenParsed(one_symbol_t[i]);
                    break;
                }
            }
            lexer->getNextChar();
        }
    };

    inline void init(Lexer *lexer) {
        ident = new Ident("ident");
        one_symbol = new OneSymbol("one-symbol");
        ignore_whitespaces = new IgnoreWhitespaces("ignore-whitespaces");

        lexer->root_rules.emplace_back(ignore_whitespaces);
        lexer->root_rules.emplace_back(one_symbol);
        lexer->root_rules.emplace_back(ident);
    }
}
