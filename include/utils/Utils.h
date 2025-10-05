#pragma once
#include <vector>

namespace ncompiler::utils {
    template<typename T>
    int in(const T &c, const std::vector<T> &array) {
        for (int i = 0; i < array.size(); ++i)
            if (array[i] == c)
                return i;
        return -1;
    }

    template<typename T>
    std::string toString(const std::vector<T> &array) {
        std::string out;

        if (!array.empty()) {
            for (int i = 0; i < array.size() - 1; ++i)
                out += array[i].toString() + ", ";
            out += array[array.size() - 1].toString();
        }

        return out;
    }

    template<typename T>
    std::string toString(const std::vector<T*> &array) {
        std::string out;

        if (!array.empty()) {
            for (int i = 0; i < array.size() - 1; ++i)
                out += array[i]->toString() + ", ";
            out += array[array.size() - 1]->toString();
        }

        return out;
    }
}
