#pragma once
#include <string>

#include "Tree.h"

namespace ncompiler {
    struct Parser;

    struct ParserRule {
        std::string name;

        explicit ParserRule(std::string &&name) : name(std::move(name)) {
        }

        explicit ParserRule(const std::string &name) : name(name) {
        }

        virtual Tree* parse(Parser *parser) = 0;

        virtual ~ParserRule() = default;
    };
}
