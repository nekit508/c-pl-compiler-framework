#include "BasicParser.h"

namespace example::parser_rules::tree {
    std::string CodeBlock::toString() const {
        return "{...}";
    }


    FunctionDecl::~FunctionDecl() {
        delete code_block;
        delete return_type;
        for (const auto &param: params)
            delete param;
    }

    std::string FunctionDecl::toString() const {
        return name + " (" + ncompiler::utils::toString(params) + ")" + " -> " + return_type->toString() + " " + code_block->toString();
    }

    void FunctionDecl::acceptChilds(ncompiler::TreeWalker *walker) {
        return_type->accept(walker);
        for (const auto &param : params)
            param->accept(walker);
        code_block->accept(walker);
    }


    std::string Ident::toString() const {
        return "type<" + name + ">";
    }


    std::string Param::toString() const {
        return name + ":" + type->toString();
    }

    void Param::acceptChilds(ncompiler::TreeWalker *walker) {
        type->accept(walker);
    }

    Param::~Param() {
        delete type;
    }
}
