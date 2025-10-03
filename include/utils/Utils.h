#pragma once
#include <algorithm>
#include <vector>

namespace ncompiler {
    inline bool in(const char &c, const std::vector<char> &array) {
        return std::any_of(array.begin(), array.end(), [&c](char ch){return ch == c;});
    }
}
