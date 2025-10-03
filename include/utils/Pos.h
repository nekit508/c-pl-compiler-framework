#pragma once
#include <string>

namespace ncompiler {
    struct Pos {
        int pos, line;

        Pos(const int pos, const int line) : pos(pos), line(line) {
        }

        [[nodiscard]] std::string toString() const {
            return "(" + std::to_string(line + 1) + ":" + std::to_string(pos) + ')';
        }

        void set(const int line, const int pos) {
            this->pos = pos;
            this->line = line;
        }

        Pos &operator=(const Pos &other) = default;
    };
}
