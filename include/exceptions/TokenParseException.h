#pragma once
#include <exception>
#include <string>

namespace ncompiler {
    struct TokenParseException : Exception {
        std::string message;

        explicit TokenParseException(std::string message): message(std::move(message)) {
        }

        [[nodiscard]] std::string whats() const noexcept override {
            return message;
        }
    };
}
