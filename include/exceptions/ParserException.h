#pragma once
#include "Exception.h"
#include "utils/Pos.h"

namespace ncompiler {
    struct ParserException : Exception {
        std::string message;

        explicit ParserException(std::string message): message(std::move(message)){
        }

        [[nodiscard]] std::string whats() const noexcept override {
            return "ParserException:(" + message + ")";
        }
    };
}
