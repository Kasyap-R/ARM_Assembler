#pragma once
#include "token.h"
#include <typeindex>
#include <unordered_map>
#include <vector>

using ValidationRule = std::vector<TokenType>;

enum class ArgFormat {
    REG_REG_REG,
    REG_REG_IMM,
    REG_IMM_REG,
};

// Specialize std::hash for ArgType
namespace std {
template <> struct hash<ArgFormat> {
    size_t operator()(const ArgFormat &argType) const {
        return static_cast<std::size_t>(argType);
    }
};
} // namespace std

const std::unordered_map<ArgFormat, ValidationRule> validationRules = {
    {ArgFormat::REG_REG_REG,
     ValidationRule{TokenType::Register, TokenType::Register,
                    TokenType::Register}},
    {ArgFormat::REG_REG_IMM,
     ValidationRule{TokenType::Register, TokenType::Register,
                    TokenType::Immediate}},
    {ArgFormat::REG_IMM_REG,
     ValidationRule{TokenType::Register, TokenType::Immediate,
                    TokenType::Register}},
};

const std::unordered_map<Mnemonic, std::vector<ArgFormat>> mnemonicsToFormats =
    {
        {Mnemonic::ADD,
         std::vector<ArgFormat>{ArgFormat::REG_REG_REG, ArgFormat::REG_REG_IMM,
                                ArgFormat::REG_IMM_REG}},
};
