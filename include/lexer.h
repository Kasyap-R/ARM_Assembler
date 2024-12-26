#pragma once

#include "assembler_state.h"
#include "token.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Lexer {

  private:
    std::unordered_map<std::string, Label> stringToLabels;

    static auto processMnemonic(const std::string &line, const int lineNum)
        -> Token;

    static auto processImmediate(const std::string &immediate,
                                 const int lineNum) -> Token;

    static auto processRegister(const std::string &argument, const int lineNum)
        -> Token;

    static auto processDirective(const std::string &directive,
                                 const int lineNum) -> std::vector<Token>;

    static auto gatherArguments(std::string &line, const size_t argStartIndex,
                                const int lineNum) -> std::vector<std::string>;

    static auto trimWhitespace(const std::string &line) -> std::string;

    static auto trimComments(const std::string &line) -> std::string;

    auto processLabel(const std::string &line, const int lineNum) -> Token;

    auto processArgument(const std::string &argument, const int lineNum)
        -> Token;

    auto processLine(std::string &line, const int lineNum)
        -> std::vector<Token>;

    void populateLabelsSet(std::unordered_set<Label> &labels);

  public:
    Lexer();
    void tokenize(const std::string &assembly, AssemblerState &state);
};
