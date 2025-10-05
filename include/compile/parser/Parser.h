#pragma once
#include <algorithm>
#include <vector>

#include "ParserRule.h"
#include "Token.h"
#include "Tree.h"
#include "exceptions/ParseFile.h"
#include "exceptions/ParserException.h"
#include "utils/Logger.h"
#include "utils/ReusableStream.h"
#include "utils/Utils.h"

namespace ncompiler {
    /** Must obtain control over tokens_stream, rules and root_rule. All used rules must be contained in rules. */
    struct Parser {
        std::map<std::string, ParserRule *> rules;
        ParserRule *root_rule;
        ReusableStream<Token> *tokens_stream;
        Logger logger;
        TokenKind eof_t;

        explicit Parser(ReusableStream<Token> *tokens_stream, const TokenKind &eof_t,
                        const bool use_debug = false) : root_rule(nullptr), tokens_stream(tokens_stream),
                                                        logger(use_debug), eof_t(eof_t) {
        }

        virtual Tree *parse() {
            return nullptr;
        }

        virtual ~Parser() {
            for (const auto &[fst, snd]: rules)
                delete snd;

            delete tokens_stream;
        }

        void addRule(ParserRule *rule) {
            rules[rule->name] = rule;
        }

        ParserRule *rule(const std::string &name) {
            return rules[name];
        }

        [[nodiscard]] Token current() const {
            return tokens_stream->get();
        }

        [[nodiscard]] Token next() const noexcept(false) {
            return tokens_stream->next();
        }

        void redo() const {
            tokens_stream->redo();
        }

        void redo(int num) const {
            tokens_stream->redo(num);
        }


        [[nodiscard]] bool probe(const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto token = next();

            return std::any_of(kinds.begin(), kinds.end(), [token](const TokenKind &kind) { return token == kind; });
        }


        [[nodiscard]] bool probeAndRedo(const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto out = probe(kinds);
            redo();
            return out;
        }


        [[nodiscard]] bool probeAndRedoIf(bool value, const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto out = probe(kinds);
            if (out == value)
                redo();
            return out;
        }


        [[nodiscard]] bool probeNot(const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto token = next();

            return std::all_of(kinds.begin(), kinds.end(), [token](const TokenKind &kind) { return token != kind; });
        }


        [[nodiscard]] bool probeNotAndRedoIf(const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto out = probeNot(kinds);
            redo();
            return out;
        }


        [[nodiscard]] bool probeNotAndRedoIf(bool value, const std::vector<TokenKind> &kinds) const noexcept(false) {
            const auto out = probeNot(kinds);
            if (out == value)
                redo();
            return out;
        }


        [[nodiscard]] bool hasAnyToken() const noexcept(false) {
            const auto token = next();
            redo();
            return token != eof_t;
        }


        Token accept(const std::vector<TokenKind> &required_token_kinds) const noexcept(false) {
            const auto token = next();

            for (TokenKind required_token_kind: required_token_kinds)
                if (required_token_kind == token.kind)
                    return token;

            throw ParseFail(
                std::string("Required any of ") + utils::toString(required_token_kinds) + std::string(", but ") + token.kind.
                name() + std::string(" provided."), token.pos);
        }


        Token except(const std::vector<TokenKind> &required_token_kinds) const noexcept(false) {
            const auto token = next();

            for (TokenKind excepted_token_kind: required_token_kinds)
                if (excepted_token_kind == token.kind)
                    throw ParseFail(
                        std::string("Excepted all of ") + utils::toString(required_token_kinds) + std::string(", but ") + token
                        .kind.name() + std::string(" provided."), token.pos);

            return token;
        }

        [[nodiscard]] Tree *parseAny(const std::vector<ParserRule *> &parse_rules) noexcept(false) {
            std::string causes;

            for (const auto &rule: parse_rules) {
                try {
                    tokens_stream->saveState();
                    const auto out = parse(rule);
                    tokens_stream->disposeState();
                    return out;
                } catch (const ParseFail &e) {
                    tokens_stream->redoState();\
                    causes += "\n" + e.whats();
                }
            }

            const auto pos = tokens_stream->next().pos;
            redo();
            std::string out;
            for (const auto &rule: parse_rules)
                out += " " + rule->name;
            throw ParseFail(
                std::string("Failed to parse any of ") + out + std::string(" at ") + pos.toString() + std::string(".") +
                causes, pos);
        }

        [[nodiscard]] Tree *parse(ParserRule *rule) noexcept(false) {
            const auto pos = next().pos;
            redo();
            try {
                logger.debug("Parsing rule " + rule->name + " at " + pos.toString() + ".");
                return rule->parse(this);
            } catch (const ParseFail &e) {
                logger.debug("Failed rule " + rule->name + " at " + pos.toString());
                throw ParseFail("Failed rule " + rule->name + " at " + pos.toString() + " cause:(" + e.whats() + ")",
                                pos);
            }
        }


        [[nodiscard]] Tree *parseCompileSource() noexcept(false) {
            try {
                return parse(root_rule);
            } catch (const ParseFail &e) {
                throw ParserException(e.whats());
            }
        }
    };
}
