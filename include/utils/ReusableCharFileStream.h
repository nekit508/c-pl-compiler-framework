#pragma once
#include <fstream>

#include "ReusableStreamVectorImpl.h"

namespace ncompiler {
    struct ReusableCharFileStream : ReusableStreamVectorImpl<char> {
        std::ifstream stream;

        explicit ReusableCharFileStream(std::ifstream inputStream) : ReusableStreamVectorImpl(),
                                                                     stream(std::move(inputStream)) {
        }

        char readNextObject() noexcept(false) override {
            char c;
            if (stream.eof() || !stream.get(c))
                throw EOSExсeption();
            return c;
        }

        ~ReusableCharFileStream() override = default;
    };
}
