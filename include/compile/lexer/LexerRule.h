#pragma once
#include <string>

namespace ncompiler {
    struct Lexer;

    struct LexerRule {
        std::string name;

        explicit LexerRule(std::string &&name) : name(std::move(name)) {
        }

        bool tryParse(Lexer *lexer) noexcept(false) {
            if (!canParse(lexer))
                return false;
            parse(lexer);
            return true;
        }

        virtual void parse(Lexer *lexer) noexcept(false) = 0;

        virtual bool canParse(Lexer *lexer) noexcept(false) = 0;

        virtual ~LexerRule() = default;
    };
}
