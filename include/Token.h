#pragma once

#include <cstdint>

#include "utils/Pos.h"

namespace ncompiler {
    inline std::map<uint64_t, std::string> token_names_map;

    struct TokenKind {
        uint64_t id;

        explicit TokenKind(std::string name, const uint64_t id) : id(id) {
            token_names_map[id] = std::move(name);
        }

        bool operator==(const TokenKind &other) const {
            return other.id == id;
        }

        bool operator!=(const TokenKind & other) const {
            return other.id != id;
        }

        [[nodiscard]] std::string name() const {
            if (id == -1)
                return "";
            return token_names_map[id];
        }

        [[nodiscard]] std::string toString() const {
            return name();
        }

        void discard() {
            id = -1;
        }
    };

    struct Token {
        TokenKind kind;
        std::string literal;
        Pos pos;

        Token(): kind("", -1), pos(0, 0) {
        }

        bool operator==(const TokenKind &other) const {
            return kind == other;
        }

        bool operator!=(const TokenKind &other) const {
            return kind != other;
        }

        [[nodiscard]] std::string toString() const {
            return kind.name() + pos.toString() + literal;
        }
    };
}
