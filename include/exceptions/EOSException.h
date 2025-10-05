#pragma once

#include "Exception.h"

namespace ncompiler {
    struct EOSExсeption : Exception {
        [[nodiscard]] std::string whats() const noexcept override {
            return "EOSExсeption:(End of stream.)";
        }
    };
}
