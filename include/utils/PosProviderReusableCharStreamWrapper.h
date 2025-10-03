#pragma once
#include <vector>

#include "PosProvider.h"
#include "ReusableStream.h"
#include "ReusableStreamWrapper.h"
#include "Utils.h"

namespace ncompiler {
    struct PosProviderReusableCharStreamWrapper : PosProvider, ReusableStreamWrapper<char> {
        explicit PosProviderReusableCharStreamWrapper(ReusableStream* stream)
            : ReusableStreamWrapper(stream) {
        }

        std::vector<char> notVisible{'\n', '\r'};

        Pos currentPos{0, 0};

        std::vector<int> linesSizes;

        [[nodiscard]] Pos getCurrentPosition() override {
            return currentPos;
        }

        void redo() override {
            auto c = get();

            if (!in(c, notVisible))
                currentPos.pos -= 1;

            if (c == '\n') {
                currentPos.pos = linesSizes.back();
                linesSizes.pop_back();
                currentPos.line -= 1;
            }

            ReusableStreamWrapper::redo();
        }

        char next() override {
            auto c = ReusableStreamWrapper::next();

            if (!in(c, notVisible))
                currentPos.pos += 1;

            if (c == '\n') {
                linesSizes.emplace_back(currentPos.pos);
                currentPos.line += 1;
                currentPos.pos = 0;
            }

            return c;
        }
    };
}
