#include "compile/lexer/Lexer.h"
#include "compile/lexer/internal/BasicRules.h"
#include "utils/PosProviderReusableCharStreamWrapper.h"
#include "utils/ReusableCharFileStream.h"

int main() {
    ncompiler::Logger logger{true};

    std::ifstream stream("file");
    ncompiler::Lexer lexer(
        new ncompiler::PosProviderReusableCharStreamWrapper(
            new ncompiler::ReusableCharFileStream(
                std::move(stream))),
        true);
    ncompiler::lexer_rules::init(&lexer);

    while (true) {
        try {
            auto token = lexer.next();
            logger.info(token.toString());
        } catch (const ncompiler::LexerExсeption &e) {
            logger.error(e.whats());
            break;
        }
    }

    return 0;
}
