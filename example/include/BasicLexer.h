#pragma once

#include "compile/lexer/Lexer.h"
#include "compile/lexer/LexerRule.h"

namespace example::lexer_rules {
    inline uint64_t TID = 1;

    namespace token {
        // general
        inline ncompiler::TokenKind ident{"ident", TID++};
        inline ncompiler::TokenKind number{"number", TID++};
        inline ncompiler::TokenKind string{"string", TID++};

        // keywords
        inline ncompiler::TokenKind def{"def", TID++};
        inline ncompiler::TokenKind returnn{"return", TID++};
        inline ncompiler::TokenKind falsee{"false", TID++};
        inline ncompiler::TokenKind truee{"true", TID++};
        inline ncompiler::TokenKind voidd{"void", TID++};

        // onesymbol tokens
        inline ncompiler::TokenKind colon{"colon", TID++};
        inline ncompiler::TokenKind l_brace{"l_brace", TID++};
        inline ncompiler::TokenKind r_brace{"r_brace", TID++};
        inline ncompiler::TokenKind l_paren{"l_paren", TID++};
        inline ncompiler::TokenKind r_paren{"r_paren", TID++};
        inline ncompiler::TokenKind l_bracket{"l_bracket", TID++};
        inline ncompiler::TokenKind r_bracket{"r_bracket", TID++};
        inline ncompiler::TokenKind gr{"gr", TID++};
        inline ncompiler::TokenKind sm{"sm", TID++};
        inline ncompiler::TokenKind eq{"eq", TID++};
        inline ncompiler::TokenKind minus{"minus", TID++};
        inline ncompiler::TokenKind plus{"plus", TID++};
        inline ncompiler::TokenKind semicolon{"semicolon", TID++};
        inline ncompiler::TokenKind comma{"comma", TID++};
    }

    inline std::vector whitespaces_l = {
        ' ', '\t', '\n', '\r'
    };

    inline std::vector string_quotes = {
        '\'', '"'
    } ;

    inline std::vector number_start_l = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    inline std::vector number_body_l = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'x'
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

    inline std::vector<std::string> keywords_l = {
        "def",
        "return",
        "false",
        "true",
        "void"
    };

    inline std::vector keywords_t = {
        token::def,
        token::returnn,
        token::falsee,
        token::truee,
        token::voidd
    };


    inline std::vector one_symbol_l = {
        ':',
        '{',
        '}',
        '(',
        ')',
        '[',
        ']',
        '>',
        '<',
        '-',
        '+',
        ';',
        ','
    };

    inline std::vector one_symbol_t = {
        token::colon,
        token::l_brace,
        token::r_brace,
        token::l_paren,
        token::r_paren,
        token::l_bracket,
        token::r_bracket,
        token::gr,
        token::sm,
        token::minus,
        token::plus,
        token::semicolon,
        token::comma
    };

    struct IgnoreWhitespaces : ncompiler::LexerRule {
        explicit IgnoreWhitespaces(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        void parse(ncompiler::Lexer *lexer) noexcept(false) override {
            while (ncompiler::utils::in(lexer->getNextChar(), whitespaces_l) != -1);
        }

        bool canParse(ncompiler::Lexer *lexer) noexcept(false) override {
            return ncompiler::utils::in(lexer->getChar(), whitespaces_l) != -1;
        }
    };

    struct Ident : ncompiler::LexerRule {
        explicit Ident(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        bool canParse(ncompiler::Lexer *lexer) noexcept(false) override {
            return ncompiler::utils::in(lexer->getChar(), ident_start_l) != -1;
        }

        void parse(ncompiler::Lexer *lexer) noexcept(false) override {
            do {
                lexer->putCurChar();

            } while (ncompiler::utils::in(lexer->getNextChar(), ident_body_l) != -1);

            if (const int i = ncompiler::utils::in(lexer->ct.literal, keywords_l); i != -1)
                lexer->tokenParsed(keywords_t[i]);
            else lexer->tokenParsed(token::ident);
        }
    };

    struct OneSymbol : ncompiler::LexerRule {
        explicit OneSymbol(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        bool canParse(ncompiler::Lexer *lexer) noexcept(false) override {
            return ncompiler::utils::in(lexer->getChar(), one_symbol_l) != -1;
        }

        void parse(ncompiler::Lexer *lexer) noexcept(false) override {
            const char c = lexer->getChar();
            for (auto i = 0; i < one_symbol_l.size(); ++i) {
                if (one_symbol_l[i] == c) {
                    lexer->putChar(c);
                    lexer->tokenParsed(one_symbol_t[i]);
                    lexer->skipChar();
                    break;
                }
            }
        }
    };

    struct Number : ncompiler::LexerRule {
        explicit Number(std::string &&name)
            : LexerRule(std::move(name)) {
        }

        bool canParse(ncompiler::Lexer *lexer) noexcept(false) override {
            return ncompiler::utils::in(lexer->getChar(), number_start_l) != -1;
        }

        void parse(ncompiler::Lexer *lexer) noexcept(false) override {
            bool x_provided = false;
            do {
                const char c = lexer->getChar();

                if (c == 'x') {
                    if (x_provided)
                        throw ncompiler::TokenParseException("Number must contain no more than one x symbol.");
                    x_provided = true;
                }

                lexer->putChar(c);
            } while (ncompiler::utils::in(lexer->getNextChar(), number_body_l) != -1);
            lexer->tokenParsed(token::number);
        }
    };

    struct String : ncompiler::LexerRule {
        char quote;

        explicit String(std::string &&name)
            : LexerRule(std::move(name)), quote(0) {
        }

        bool canParse(ncompiler::Lexer *lexer) noexcept(false) override {
            const char c = lexer->getChar();

            if (ncompiler::utils::in(c, string_quotes) == -1) return false;

            quote = c;
            return true;
        }

        void parse(ncompiler::Lexer *lexer) noexcept(false) override {
            while (lexer->getNextChar() != quote) {
                lexer->putCurChar();
            }
            lexer->skipChar();
            lexer->tokenParsed(token::string);
        }
    };

    inline void init(ncompiler::Lexer *lexer) {
        auto *ident = new Ident("ident");
        auto *one_symbol = new OneSymbol("one-symbol");
        auto *ignore_whitespaces = new IgnoreWhitespaces("ignore-whitespaces");
        auto *number = new Number("number");
        auto *string = new String("string");

        lexer->root_rules.emplace_back(ignore_whitespaces);
        lexer->root_rules.emplace_back(one_symbol);
        lexer->root_rules.emplace_back(ident);
        lexer->root_rules.emplace_back(number);
        lexer->root_rules.emplace_back(string);
    }
}
