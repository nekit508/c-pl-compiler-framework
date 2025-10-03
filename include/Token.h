#pragma once

#include "TokenKind.h"
#include "utils/Pos.h"

namespace ncompiler {
    struct Token {
        const TokenKind *kind;
        std::string literal;
        Pos pos;

        Token(): kind(nullptr), pos(0, 0) {}

        bool operator==(const TokenKind& other) const {
            return *kind == other;
        }

        [[nodiscard]] std::string toString() const {
            return kind->name + pos.toString() + literal;
        }
    };
}
