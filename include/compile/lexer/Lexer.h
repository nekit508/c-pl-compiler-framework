#pragma once

#include <map>

#include "LexerRule.h"
#include "Token.h"
#include "exceptions/EOSException.h"
#include "exceptions/LexerException.h"
#include "exceptions/TokenParseException.h"
#include "utils/Logger.h"
#include "utils/PosProviderReusableCharStreamWrapper.h"
#include "utils/ReusableStreamVectorImpl.h"

namespace ncompiler {
    /** Must obtaint control over reader and root_rules. */
    struct Lexer : ReusableStreamVectorImpl<Token> {
        PosProviderReusableCharStreamWrapper *reader;
        std::vector<LexerRule *> root_rules;
        Token ct;
        bool token_parsed = false;
        Logger logger;
        bool eof = false;

        TokenKind eof_t{"eof", 0};

        explicit Lexer(PosProviderReusableCharStreamWrapper *reader, const bool use_debug = false) : reader(reader),
            logger(use_debug) {
        }

        void saveCharState() const {
            reader->saveState();
        }

        void disposeCharState() const {
            reader->disposeState();
        }

        void redoCharState() const {
            reader->redoState();
        }

        [[nodiscard]] char getChar() const {
            return reader->get();
        }

        void putChar(char c) {
            ct.literal.push_back(c);
        }

        void putCurChar() {
            putChar(getChar());
        }

        void redoChar() const {
            reader->redo();
        }

        char getNextChar() const {
            // NOLINT(*-use-nodiscard)
            return reader->next();
        }

        void skipChar() const {
            reader->next();
        }

        [[nodiscard]] Pos pos() const {
            return reader->getCurrentPosition();
        }

        void tokenParsed() {
            token_parsed = true;
        }

        void tokenParsed(const TokenKind &kind) {
            tokenParsed();
            ct.kind = kind;
        }

        Token readNextObject() noexcept(false) override {
            parseNextToken();
            return ct;
        }

        bool tryParse(LexerRule *rule) {
            return rule->tryParse(this);
        }

        bool tryParseAny(LexerRule *rule) {
            return rule->tryParse(this);
        }

        void parseNextToken() noexcept(false) {
            try {
                ct.literal.erase();
                ct.kind.discard();
                token_parsed = false;
                ct.pos = reader->getCurrentPosition();
                
                if (eof) {
                    ct.kind = eof_t;
                    return;
                }

                if (reader->getPos() == -1) {
                    const auto c = reader->next();
                    if (c == static_cast<char>(65279))
                        logger.error(
                            "Probably you are using UTF with BOM that is not supported. " + reader->getCurrentPosition()
                            .toString());
                }

                for (const auto &rule: root_rules) {
                    try {
                        if (parseIfCan(rule) && token_parsed)
                            break;
                    } catch (const TokenParseException &fail) {
                        throw LexerExсeption(fail.whats(), reader->getCurrentPosition());
                    }
                }

                if (!token_parsed)
                    throw LexerExсeption("Unknown token kind. ", reader->getCurrentPosition());
            } catch (const EOSExсeption &e) {
                if (!token_parsed)
                    throw LexerExсeption(e.whats(), reader->getCurrentPosition());
                eof = true;
            }
        }

        [[nodiscard]] bool parseIfCan(LexerRule *rule) const noexcept(false) {
            return rule->tryParse(const_cast<Lexer *>(this));
        }

        ~Lexer() override {
            delete reader;
            for (const auto root_rule: root_rules)
                delete root_rule;
        }
    };
}
