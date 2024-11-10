#pragma once

#include "token.h"
#include <string>
#include <unordered_map>
#include <vector>

class Lexer {
  private:
    static const std::unordered_map<std::string, Mnemonic> stringToMnemonic;
    static const std::unordered_map<std::string, Register> stringToRegister;
    static const std::unordered_map<std::string, Directive> stringToDirective;

    const std::string &assembly;
    std::unordered_map<std::string, Label> labels;

    static Token processMnemonic(const std::string &line, int lineNum);
    static Token processImmediate(const std::string &immediate, int lineNum);
    static Token processRegister(const std::string &argument, int lineNum);
    static std::vector<Token> processDirective(const std::string &directive,
                                               int lineNum);
    Token processLabel(const std::string &line, int lineNum);
    Token processArgument(const std::string &argument, int lineNum);
    static auto gatherArguments(std::string &line, size_t argStartIndex,
                                int lineNum) -> std::vector<std::string>;

    static std::string trimWhitespace(const std::string &line);
    static std::string trimComments(const std::string &line);

  public:
    Lexer(std::string &assembly);
    std::vector<Token> tokenize();
};
