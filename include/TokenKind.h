#pragma once
#include <cstdint>
#include <string>

namespace ncompiler {
    struct TokenKind {
        std::string name;
        uint64_t id;

        TokenKind(std::string name, const uint64_t id) : name(std::move(name)), id(id) {}

        bool operator==(const TokenKind &other) const {
            return other.id == id;
        }
    };
}
