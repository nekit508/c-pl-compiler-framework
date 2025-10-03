#pragma once
#include "CompileSource.h"
#include "utils/ReusableCharFileStream.h"

namespace ncompiler {
    struct FileCompileSource : CompileSource {
        ReusableCharFileStream stream;

        explicit FileCompileSource(const std::string& fileName) : stream(ReusableCharFileStream(std::ifstream(fileName))) {}

        [[nodiscard]] const ReusableStream<char> *getInputStream() const override {
            return &stream;
        }
    };
}
