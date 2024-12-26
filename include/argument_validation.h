#pragma once
#include "token.h"
#include <unordered_map>
#include <vector>

using ValidationRule = std::vector<TokenType>;

enum class ArgFormat {
    REG_REG_REG,
    REG_REG_IMM,
    REG_IMM_REG,
    REG_REG,
    REG_IMM,
};

// Specialize std::hash for ArgFormat
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
    {ArgFormat::REG_REG,
     ValidationRule{TokenType::Register, TokenType::Register}},
    {ArgFormat::REG_IMM,
     ValidationRule{TokenType::Register, TokenType::Immediate}}};

const std::unordered_map<Mnemonic, std::vector<ArgFormat>> mnemonicsToFormats =
    {{Mnemonic::ADD,
      std::vector<ArgFormat>{ArgFormat::REG_REG_REG, ArgFormat::REG_REG_IMM,
                             ArgFormat::REG_IMM_REG}},
     {Mnemonic::MOV,
      std::vector<ArgFormat>{ArgFormat::REG_REG, ArgFormat::REG_IMM}}};
