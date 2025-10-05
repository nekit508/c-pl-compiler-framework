#include "BasicLexer.h"
#include "BasicParser.h"
#include "compile/lexer/Lexer.h"
#include "compile/parser/Parser.h"
#include "utils/PosProviderReusableCharStreamWrapper.h"
#include "utils/ReusableCharFileStream.h"

void process_file(const std::string &file) {
    ncompiler::Logger logger{true};

    const auto lexer = new ncompiler::Lexer(
        new ncompiler::PosProviderReusableCharStreamWrapper(new ncompiler::ReusableCharFileStream(std::ifstream(file))),
        true);
    ncompiler::Parser parser(lexer, lexer->eof_t, true);
    example::lexer_rules::init(lexer);
    example::parser_rules::init(&parser);

    try {
        const auto tree = parser.parseCompileSource();
        logger.info(tree->toString());
        delete tree;
    } catch (const ncompiler::ParserException &e) {
        logger.error(e.whats());
    }
}

int main() {
    std::string file;
    while (true) {
        std::getline(std::cin, file);
        if (file == ":q") break;
        process_file(file);
    }
    return 0;
}
