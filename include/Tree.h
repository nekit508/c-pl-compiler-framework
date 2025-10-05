#pragma once
#include <cstdint>
#include <string>
#include <utility>

namespace ncompiler {
    inline std::map<uint64_t, std::string> tree_names_map;

    struct TreeKind {
        uint64_t id;

        explicit TreeKind(std::string name, const uint64_t id) : id(id) {
            tree_names_map[id] = std::move(name);
        }

        bool operator==(const TreeKind &other) const {
            return other.id == id;
        }

        bool operator!=(const TreeKind &other) const {
            return other.id != id;
        }

        [[nodiscard]] std::string name() const {
            if (id == -1)
                return "";
            return tree_names_map[id];
        }

        [[nodiscard]] std::string toString() const {
            return name();
        }

        void discard() {
            id = -1;
        }
    };

    struct Tree;

    struct TreeWalker {
        virtual ~TreeWalker() = default;

        virtual void enter(Tree *tree) = 0;

        virtual void process(Tree *tree) = 0;

        virtual void exit(Tree *tree) = 0;
    };

    struct Tree {
        const TreeKind kind;

        explicit Tree(const TreeKind kind) : kind(kind) {
        }

        virtual ~Tree() = default;

        bool operator==(const TreeKind &other) const {
            return kind == other;
        }

        bool operator!=(const TreeKind &other) const {
            return kind != other;
        }

        virtual void accept(TreeWalker *walker) {
            walker->enter(this);
            acceptChilds(walker);
            walker->process(this);
            walker->exit(this);
        }

        virtual void acceptChilds(TreeWalker *walker) {
        }

        [[nodiscard]] virtual std::string toString() const {
            return kind.name();
        }
    };
}
