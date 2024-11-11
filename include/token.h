#pragma once

#include "mnemonic.h"
#include "register.h"

#include <ostream>
#include <string>
#include <variant>

enum class Directive {
    GLOBAL,
    DATA,
    TEXT,
};

struct Label {
    std::string val;
};

// Specialize std::hash for Label
namespace std {
template <> struct hash<Label> {
    size_t operator()(const Label &label) const {
        // Combine hash values of members
        return hash<std::string>()(label.val);
    }
};
} // namespace std

struct Immediate {
    int val;
};

enum class TokenType {
    Mnemonic,
    Register,
    Directive,
    Label,
    Immediate,
    Newline,
};

struct Token {
    TokenType type;
    std::variant<std::monostate, Mnemonic, Register, Directive, Label,
                 Immediate>
        token;

    // Factory Methods to create Tokens
    static Token createMnemonic(Mnemonic mnemonic) {
        return Token{TokenType::Mnemonic, mnemonic};
    }

    static Token createRegister(Register reg) {
        return Token{TokenType::Register, reg};
    }

    static Token createDirective(Directive directive) {
        return Token{TokenType::Directive, directive};
    }

    static Token createLabel(Label label) {
        return Token{TokenType::Label, label};
    }

    static Token createImmediate(Immediate immediate) {
        return Token{TokenType::Immediate, immediate};
    }

    static Token createNewline() {
        return Token{TokenType::Newline, std::monostate()};
    }
};

// Equality operators (inline must be used since this is a header file)
inline bool operator==(const Label &lhs, const Label &rhs) {
    return lhs.val == rhs.val;
}

inline bool operator==(const Immediate &lhs, const Immediate &rhs) {
    return lhs.val == rhs.val;
}

inline bool operator==(const Token &lhs, const Token &rhs) {
    if (lhs.type != rhs.type) {
        return false;
    }
    return lhs.token ==
           rhs.token; // Uses the functions defined above for comparing structs
}

// Stream output operator for debugging
inline std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "Token(";
    switch (token.type) {
    case TokenType::Mnemonic:
        os << "Mnemonic";
        break;
    case TokenType::Register:
        os << "Register";
        break;
    case TokenType::Directive:
        os << "Directive";
        break;
    case TokenType::Label:
        os << "Label: " << std::get<Label>(token.token).val;
        break;
    case TokenType::Immediate:
        os << "Immediate: " << std::get<Immediate>(token.token).val;
        break;
    case TokenType::Newline:
        os << "Newline";
    }
    os << ")\n";
    return os;
}
