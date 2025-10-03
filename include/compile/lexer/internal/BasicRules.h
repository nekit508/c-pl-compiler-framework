#pragma once
#include "compile/lexer/Lexer.h"
#include "compile/lexer/LexerRule.h"

namespace ncompiler::lexer_rules {
    inline auto ident = new TokenKind("ident", 2);

    inline LexerRule *identRule, *numberRule;



    inline void init(Lexer &lexer) {

        lexer.root_rules.emplace_back(identRule);
    }
}
