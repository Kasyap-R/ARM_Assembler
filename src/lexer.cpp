#include "lexer.h"
#include "lexer_constants.h"
#include "token.h"
#include "util.h"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

Lexer::Lexer() : stringToLabels({}) {};

void Lexer::tokenize(const std::string &assembly,
                     AssemblerState &assemblerState) {
    std::istringstream stream(assembly);
    std::string line;
    int lineNum = 0;

    while (std::getline(stream, line)) {
        // Process line by line
        lineNum++;

        std::vector<Token> tokensInLine = Lexer::processLine(line, lineNum);

        assemblerState.tokens.insert(assemblerState.tokens.end(),
                                     tokensInLine.begin(), tokensInLine.end());

        if (!assemblerState.tokens.empty() && stream.peek() != EOF) {
            assemblerState.tokens.push_back(Token::createNewline());
        }
    }
}

auto Lexer::processLine(std::string &line, const int lineNum)
    -> std::vector<Token> {

    std::vector<Token> tokens = {};
    line = Lexer::trimComments(Lexer::trimWhitespace(line));

    if (line.empty()) {
        return tokens;
    }
    if (line[0] == '.') {
        return Lexer::processDirective(line, lineNum);
    }
    if (line.back() == ':') {
        tokens.push_back(Lexer::processLabel(line, lineNum));
        return tokens;
    }

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
        tokens.push_back(this->processArgument(argument, lineNum));
    }

    return tokens;
}

auto Lexer::processArgument(const std::string &argument, const int lineNum)
    -> Token {
    if (util::isValidKey(argument, this->stringToLabels)) {
        return Token::createLabel(this->stringToLabels.at(argument));
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

auto Lexer::processMnemonic(const std::string &line, const int lineNum)
    -> Token {
    size_t firstWhitespaceIdx = line.find(' ');

    if (firstWhitespaceIdx == std::string::npos) {
        throw std::runtime_error("Expected arguments after mnemonic on line " +
                                 std::to_string(lineNum));
    }

    std::string mnemonic = line.substr(0, firstWhitespaceIdx);
    if (!util::isValidKey(mnemonic, stringToMnemonic)) {
        throw std::runtime_error("Expected mnemonic as first string at line: " +
                                 std::to_string(lineNum));
    }
    return Token::createMnemonic(stringToMnemonic.at(mnemonic));
}

auto Lexer::gatherArguments(std::string &line, const size_t argStartIndex,
                            int lineNum) -> std::vector<std::string> {
    std::vector<std::string> arguments;
    std::string currentArgument;
    line.push_back(','); // Forces last argument to be pushed to arguments list
    for (size_t i = argStartIndex; i < line.size(); i++) {
        if (line[i] == ' ') {
            continue;
        }
        if (line[i] == ',') {
            if (currentArgument.empty()) {
                throw std::runtime_error(
                    "Expected argument (label, register, or immediate) "
                    "before/after comma on line " +
                    std::to_string(lineNum));
            }
            arguments.push_back(currentArgument);
            currentArgument = "";
        } else {
            currentArgument.push_back(line[i]);
        }
    }
    line.pop_back(); // Remove that last comma
    return arguments;
}

auto Lexer::processLabel(const std::string &line, const int lineNum) -> Token {
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
    this->stringToLabels.insert({trimmedLine, label});
    return Token::createLabel(label);
}

auto Lexer::processImmediate(const std::string &immediate, const int lineNum)
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

auto Lexer::processRegister(const std::string &argument, const int lineNum)
    -> Token {
    if (!util::isValidKey(argument, stringToRegister)) {
        throw std::runtime_error("Invalid register on line " +
                                 std::to_string(lineNum) + ": " + argument);
    }
    return Token::createRegister(stringToRegister.at(argument));
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
    if (!util::isValidKey(directiveLiteral, stringToDirective)) {
        throw std::runtime_error("Invalid directive at line " +
                                 std::to_string(lineNum) + ": " + directive);
    }

    if (firstWhitespaceIdx == std::string::npos) {
        return std::vector{
            Token::createDirective(stringToDirective.at(directiveLiteral))};
    }

    throw std::runtime_error("Directive parsing not fully implemented");
}

auto Lexer::trimWhitespace(const std::string &line) -> std::string {
    size_t start =
        line.find_first_not_of(" \n\t\r"); // Find first non whitespace
    if (start == std::string::npos) {
        return "";
    }
    size_t end = line.find_last_not_of(" \n\t\r");

    return line.substr(start, end - start + 1);
}

auto Lexer::trimComments(const std::string &line) -> std::string {
    size_t end = std::string::npos;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ';' ||
            ((i + 1) < line.size() && line.substr(i, 2) == "//")) {
            end = i;
            break;
        }
    }
    if (end == 0) {
        return "";
    }
    return line.substr(0, end);
}
