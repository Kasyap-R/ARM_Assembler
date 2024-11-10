#pragma once

#include "token.h"
#include <string>
#include <unordered_map>
#include <vector>

class Lexer {
  private:
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

    template <typename MapType>
    static auto isValidKey(const std::string &key, const MapType &map) -> bool;

    static auto processLabel(const std::string &line, const int lineNum,
                             std::unordered_map<std::string, Label> &labels)
        -> Token;

    static auto
    processArgument(const std::string &argument, const int lineNum,
                    const std::unordered_map<std::string, Label> &labels)
        -> Token;

  public:
    static auto tokenize(const std::string &assembly,
                         std::unordered_map<std::string, Label> &labels)
        -> std::vector<Token>;
};
