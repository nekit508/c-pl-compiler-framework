#pragma once
#include <exception>

#include "Exception.h"

namespace ncompiler {
    struct EOSExсeption : Exception {
        [[nodiscard]] std::string whats() const noexcept override {
            return "End of stream.";
        }
    };
}
