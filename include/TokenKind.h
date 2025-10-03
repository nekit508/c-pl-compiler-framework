#pragma once
#include <cstdint>
#include <string>

namespace ncompiler {
    static uint64_t TokenKindID = 0;

    struct TokenKind {
        std::string name;
        uint64_t id;

        explicit TokenKind(std::string name) : name(std::move(name)), id(TokenKindID++) {
        }

        bool operator==(const TokenKind &other) const {
            return other.id == id;
        }
    };
}
