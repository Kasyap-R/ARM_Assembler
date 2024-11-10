#include "lexer.h"
#include "token.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const std::unordered_map<std::string, Mnemonic> Lexer::stringToMnemonic = {
    {"add", Mnemonic::ADD}, {"sub", Mnemonic::SUB},   {"mov", Mnemonic::MOV},
    {"j", Mnemonic::JUMP},  {"jump", Mnemonic::JUMP},
};

const std::unordered_map<std::string, Directive> Lexer::stringToDirective = {
    {"global", Directive::GLOBAL},
    {"data", Directive::DATA},
    {"text", Directive::TEXT}};

const std::unordered_map<std::string, Register> Lexer::stringToRegister =
    { // X registers
        {"x1", Register::X1},
        {"x2", Register::X2},
        {"x3", Register::X3},
        {"x4", Register::X4},
        {"x5", Register::X5},
        {"x6", Register::X6},
        {"x7", Register::X7},
        {"x8", Register::X8},
        {"x9", Register::X9},
        {"x10", Register::X10},
        {"x11", Register::X11},
        {"x12", Register::X12},
        {"x13", Register::X13},
        {"x14", Register::X14},
        {"x15", Register::X15},
        {"x16", Register::X16},
        {"x17", Register::X17},
        {"x18", Register::X18},
        {"x19", Register::X19},
        {"x20", Register::X20},
        {"x21", Register::X21},
        {"x22", Register::X22},
        {"x23", Register::X23},
        {"x24", Register::X24},
        {"x25", Register::X25},
        {"x26", Register::X26},
        {"x27", Register::X27},
        {"x28", Register::X28},
        {"x29", Register::X29},
        {"x30", Register::X30},
        {"x31", Register::X31},
        {"x32", Register::X32},

        // W registers
        {"w1", Register::W1},
        {"w2", Register::W2},
        {"w3", Register::W3},
        {"w4", Register::W4},
        {"w5", Register::W5},
        {"w6", Register::W6},
        {"w7", Register::W7},
        {"w8", Register::W8},
        {"w9", Register::W9},
        {"w10", Register::W10},
        {"w11", Register::W11},
        {"w12", Register::W12},
        {"w13", Register::W13},
        {"w14", Register::W14},
        {"w15", Register::W15},
        {"w16", Register::W16},
        {"w17", Register::W17},
        {"w18", Register::W18},
        {"w19", Register::W19},
        {"w20", Register::W20},
        {"w21", Register::W21},
        {"w22", Register::W22},
        {"w23", Register::W23},
        {"w24", Register::W24},
        {"w25", Register::W25},
        {"w26", Register::W26},
        {"w27", Register::W27},
        {"w28", Register::W28},
        {"w29", Register::W29},
        {"w30", Register::W30},
        {"w31", Register::W31},
        {"w32", Register::W32}};

Lexer::Lexer(std::string &assembly) : assembly{assembly} {}

auto Lexer::tokenize() -> std::vector<Token> {
    std::istringstream stream(this->assembly);
    std::string line;
    std::vector<Token> tokens;
    int lineNum = 0;

    while (std::getline(stream, line)) {
        // Process line by line
        lineNum++;
        if (line[0] == '.') {
            std::vector directiveTokens =
                Lexer::processDirective(line, lineNum);
            tokens.insert(tokens.end(), directiveTokens.begin(),
                          directiveTokens.end());
        } else if (line.back() == ':') {
            tokens.push_back(this->processLabel(line, lineNum));
        } else if (line[0] == ';' or
                   (line.size() >= 2 && line.substr(0, 2) == "//")) {
            // Ignore comments
            continue;
        } else {
            // TODO: Check if the line contains a label for macros

            // process mnemonic
            tokens.push_back(Lexer::processMnemonic(line, lineNum));

            // Then process arguments and turn them into the appropriate tokens
            size_t argStartIndex = line.find(' ') + 1;
            if (argStartIndex >= line.size()) {
                throw std::runtime_error("Expected arguments at line " +
                                         std::to_string(lineNum));
            }
            std::vector<std::string> arguments =
                Lexer::gatherArguments(line, argStartIndex, lineNum);

            for (const auto &argument : arguments) {
                tokens.push_back(processArgument(argument, lineNum));
            }
        }
    }

    return tokens;
}

auto Lexer::processArgument(const std::string &argument, int lineNum) -> Token {
    if (this->labels.find(argument) != this->labels.end()) {
        return Token::createLabel(this->labels[argument]);
    }
    if (argument[0] == '#') {
        return Lexer::processImmediate(argument, lineNum);
    }
    if (argument[0] == 'w' || argument[0] == 'x') {
        return Lexer::processRegister(argument, lineNum);
    }
    throw std::runtime_error("Invalid argument on line " +
                             std::to_string(lineNum) + ": " + argument);
}

auto Lexer::processMnemonic(const std::string &line, int lineNum) -> Token {
    size_t firstWhitespaceIdx = line.find(' ');

    if (firstWhitespaceIdx == std::string::npos) {
        throw std::runtime_error("Expected arguments after mnemonic on line " +
                                 std::to_string(lineNum));
    }

    std::string mnemonic = line.substr(0, firstWhitespaceIdx);
    if (Lexer::stringToMnemonic.find(mnemonic) ==
        Lexer::stringToMnemonic.end()) {
        throw std::runtime_error("Expected mnemonic as first string at line: " +
                                 std::to_string(lineNum));
    }
    return Token::createMnemonic(Lexer::stringToMnemonic.at(mnemonic));
}

auto Lexer::gatherArguments(std::string &line, size_t argStartIndex,
                            int lineNum) -> std::vector<std::string> {
    std::vector<std::string> arguments;
    std::string currentArgument;
    line.push_back(','); // Forces last argument to be pushed to arguments list
    for (size_t i = argStartIndex; i < line.size(); i++) {
        if (line[i] == ',') {
            if (currentArgument.empty()) {
                throw std::runtime_error(
                    "Expected argument (label, register, or immediate) "
                    "before/after comma on line " +
                    std::to_string(lineNum));
            }
            arguments.push_back(currentArgument);
            currentArgument = "";
        } else if (line[i] == ' ') {
            continue;
        } else {
            currentArgument.push_back(line[i]);
        }
    }
    line.pop_back(); // Remove that last comma
    return arguments;
}

auto Lexer::processLabel(const std::string &line, int lineNum) -> Token {
    // Verify that the label starts with either an underscore or alphabetic
    // character
    if (line[0] != '_' && (std::isalpha(line[0]) == 0)) {
        throw std::runtime_error(
            "Expected label to start with an underscore or alphabetic "
            "character at line " +
            std::to_string(lineNum));
    }

    // Verify none of the characters are non alphanumeric/underscores
    for (auto byte : line) {
        if ((std::isalnum(byte) == 0) && byte == '_') {
            throw std::runtime_error(
                "Unexpected character in label (only alphanumeric and "
                "underscores allowed) at line " +
                std::to_string(lineNum));
        }
        if (byte == ' ') {
            throw std::runtime_error(
                "No whitespaces allowed in label name at line " +
                std::to_string(lineNum));
        }
    }
    std::string trimmedLine = line.substr(0, line.size() - 1);
    Label label{trimmedLine};
    this->labels.insert({trimmedLine, label});
    return Token::createLabel(label);
}

auto Lexer::processImmediate(const std::string &immediate, int lineNum)
    -> Token {
    // First we must determine if this number is in decimal, hex, or binary
    // Remember first char in the immediate is a hashtag
    int immediateVal = 0;
    try {
        if (immediate.size() >= 3 && immediate.substr(1, 2) == "0b") {
            immediateVal = std::stoi(immediate.substr(3), nullptr, 2);
        } else {
            immediateVal =
                std::stoi(immediate.substr(1), nullptr,
                          0); // Auto-detect base (binary not supported)
        }
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error("Invalid immediate value on line " +
                                 std::to_string(lineNum) + ": " + immediate +
                                 "\n");
    } catch (const std::out_of_range &e) {
        throw std::runtime_error("Immediate value out of range on line " +
                                 std::to_string(lineNum));
    }
    return Token::createImmediate(Immediate{immediateVal});
}

auto Lexer::processRegister(const std::string &argument, int lineNum) -> Token {
    if (Lexer::stringToRegister.find(argument) ==
        Lexer::stringToRegister.end()) {
        throw std::runtime_error("Invalid register on line " +
                                 std::to_string(lineNum));
    }
    return Token::createRegister(Lexer::stringToRegister.at(argument));
}

// TODO: Add directive processing. Issue is that arguments for directives don't
// follow same patterns as other arguments so it's a pain to deal with.
auto Lexer::processDirective(const std::string &directive, int lineNum)
    -> std::vector<Token> {
    size_t firstWhitespaceIdx = directive.find(' ');
    std::string directiveLiteral = directive.substr(
        1, firstWhitespaceIdx == std::string::npos ? std::string::npos
                                                   : firstWhitespaceIdx - 1);
    // Ensure the directive is valid
    if (Lexer::stringToDirective.find(directiveLiteral) ==
        Lexer::stringToDirective.end()) {
        throw std::runtime_error("Invalid directive at line " +
                                 std::to_string(lineNum) + ": " + directive);
    }

    if (firstWhitespaceIdx == std::string::npos) {
        return std::vector{Token::createDirective(
            Lexer::stringToDirective.at(directiveLiteral))};
    }
}
