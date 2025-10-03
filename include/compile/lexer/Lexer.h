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
    /** Must obtaint control over posProvider and stream. */
    struct Lexer : ReusableStreamVectorImpl<Token> {
        PosProviderReusableCharStreamWrapper *reader;
        std::vector<std::string> root_rules;
        std::map<std::string, LexerRule> rules;
        Token ct;
        bool token_parsed = false;
        Logger logger;

        explicit Lexer(PosProviderReusableCharStreamWrapper *reader, const bool use_debug = false) : reader(reader), logger(use_debug) {
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

        void redoChar() const {
            reader->redo();
        }

        [[nodiscard]] char getNextChar() const {
            return reader->next();
        }

        [[nodiscard]] Pos pos() const {
            return reader->getCurrentPosition();
        }

        void tokenParsed() {
            token_parsed = true;
        }

        Token readNextObject() noexcept(false) override {
            parseNextToken();
            return ct;
        }

        bool tryParse(const std::string &name) {
            return rules[name].tryParse(this);
        }

        void parseNextToken() noexcept(false) {
            try {
                token_parsed = false;
                ct.kind = nullptr;
                ct.literal.erase();

                const auto c = reader->next();

                ct.pos = reader->getCurrentPosition();

                for (const auto &rule: root_rules) {
                    try {
                        if (parseIfCan(rules[rule]))
                            break;
                    } catch (const TokenParseException &fail) {
                        throw LexerExсeption(fail.whats(), reader->getCurrentPosition());
                    }
                }

                if (!token_parsed) {
                    if (c == static_cast<char>(65279))
                        throw LexerExсeption("Probably you are using UTF with BOM that is not supported.",
                                             reader->getCurrentPosition());
                    throw LexerExсeption("Unknown token kind.", reader->getCurrentPosition());
                }
            } catch (const EOSExсeption &e) {
                if (!token_parsed)
                    throw LexerExсeption(e.whats(), reader->getCurrentPosition());
            }
        }

        [[nodiscard]] bool parseIfCan(LexerRule &rule) const noexcept(false) {
            return rule.tryParse(const_cast<Lexer *>(this));
        }

        ~Lexer() override {
            delete reader;
        }
    };
}
