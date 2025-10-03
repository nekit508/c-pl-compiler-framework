#pragma once
#include <string>

namespace ncompiler {
    struct Exception : std::exception {
        ~Exception() override = default;

        [[nodiscard]] const char *what() const noexcept override {
            return whats().c_str();
        }

        [[nodiscard]] virtual std::string whats() const noexcept = 0;
    };
}
