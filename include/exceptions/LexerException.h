#pragma once
#include <exception>
#include <string>

#include "Exception.h"
#include "utils/Pos.h"

namespace ncompiler {
    struct LexerExсeption : Exception {
        std::string message;
        Pos pos;

        LexerExсeption(std::string message, Pos pos): message(std::move(message)), pos(pos) {
        }

        [[nodiscard]] std::string whats() const noexcept override {
            return message + pos.toString();
        }
    };
}
