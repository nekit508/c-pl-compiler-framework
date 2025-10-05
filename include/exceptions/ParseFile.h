#pragma once
#include "Exception.h"
#include "utils/Pos.h"

namespace ncompiler {
    struct ParseFail : Exception {
        std::string message;
        Pos pos;

        explicit ParseFail(std::string message, Pos pos): message(std::move(message)), pos(pos) {
        }

        [[nodiscard]] std::string whats() const noexcept override {
            return "ParseFail:(" + message + ")" + pos.toString();
        }
    };
}
