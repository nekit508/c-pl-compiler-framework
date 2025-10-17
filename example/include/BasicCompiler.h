#pragma once
#include <fstream>

#include "BasicParser.h"
#include "Tree.h"

#define PROCESS(type, code) void process(type *tree) {\
    if (tree == nullptr) this->logger.error("Received nullptr.");\
    code\
}

using namespace example::parser_rules::tree;

namespace example {
    struct Compiler {
        ncompiler::Logger logger;
        ncompiler::Tree *tree;
        std::fstream ofs;

        explicit Compiler(ncompiler::Tree *tree): logger(true) {
            this->tree = tree;
        }

        void compile(const std::string &output_file) {
            ofs.open(output_file, std::ios_base::out | std::ios_base::binary);
            if (!ofs.good())
                logger.error("Failed to open file " + output_file + " rdstate:(" + std::to_string(ofs.rdstate()) + ").");
            process(tree);
        }

        PROCESS(ncompiler::Tree,
                {
                    if (tree->kind == kind::function_decl) {
                        process(dynamic_cast<FunctionDecl*>(tree));
                    }
                })

        PROCESS(FunctionDecl,
            {
                ofs << tree->name.size();
                ofs.write(tree->name.data(), tree->name.size());
            })
    };
}
