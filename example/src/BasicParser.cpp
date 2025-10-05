#include "BasicParser.h"

namespace example::parser_rules::tree {
    std::string ReturnStatement::toString() const {
        return "return " + value->toString();
    }

    ReturnStatement::~ReturnStatement() {
        delete value;
    }


    std::string ConstValue::toString() const {
        return "<" + to_string(type) + ">[" + std::to_string(reinterpret_cast<size_t>(value)) + "]";
    }


    std::string CodeBlock::toString() const {
        static int identation_level = 0;
        identation_level++;

        std::string out, nl = "\n";
        for (int i = 0; i < identation_level*4; ++i) nl.push_back(' ');

        out += nl + ncompiler::utils::toString(statements, nl);

        identation_level--;
        return out;
    }

    CodeBlock::~CodeBlock() {
        for (const auto &statement : statements)
            delete statement;
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
