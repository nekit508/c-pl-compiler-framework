#pragma once
#include "utils/ReusableStream.h"

namespace ncompiler {
    struct CompileSource {
        virtual ~CompileSource() = default;

        [[nodiscard]] virtual const ReusableStream<char>* getInputStream() const = 0;
    };
}
