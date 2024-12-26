#include "parser.h"
#include "argument_validation.h"
#include "instruction.h"
#include "token.h"
#include "util.h"
#include <span>
#include <stdexcept>

Parser::Parser() : pc{0} {};

void Parser::parse(AssemblerState &assemblerState) {
    std::vector<Token> currLine;

    // Makes it easier to delimit by newline
    assemblerState.tokens.push_back(Token::createNewline());

    for (auto &token : assemblerState.tokens) {
        if (token.type == TokenType::Newline) {
            if (!currLine.empty()) {
                assemblerState.instructions.push_back(this->parseInstruction(
                    currLine, assemblerState.labelToAddress));
            }
            currLine.clear();
        } else {
            currLine.push_back(token);
        }
    }

    assemblerState.tokens.pop_back();
}

auto Parser::parseInstruction(std::vector<Token> &tokens, LabelMap &labelMap)
    -> Instruction {
    // TODO: Add line nums to token for better errors
    const Token &firstToken = tokens[0];

    switch (firstToken.type) {
    case TokenType::Label: {
        if (tokens.size() > 1) {
            throw std::runtime_error("Unexpected tokens following label");
        }
        labelMap.insert({std::get<Label>(firstToken.token), this->pc});
        break;
    }

    case TokenType::Mnemonic: {
        // This is a machine instruction
        const std::span<const Token> arguments(tokens.data() + 1,
                                               tokens.size() - 1);
        if (!Parser::validateMnemonicArguments(
                std::get<Mnemonic>(tokens[0].token), arguments)) {
            throw std::runtime_error("Invalid arguments for a mnemonic");
        }
        this->pc += 4;
        break;
    }

    case TokenType::Directive: {
        throw std::runtime_error("Directive instructions not yet supported");
        break;
    }
    case TokenType::Newline:
    case TokenType::Register:
    case TokenType::Immediate: {
        throw std::runtime_error("Invalid instruction");
    }
    };
    return Instruction{tokens};
}

auto Parser::validateMnemonicArguments(Mnemonic mnemonic,
                                       const std::span<const Token> &args)
    -> bool {
    if (!util::isValidKey(mnemonic, mnemonicsToFormats)) {
        throw std::runtime_error("Unsupported mnemonic found while parsing");
    }

    const std::vector<ArgFormat> &supportedArgFormats =
        mnemonicsToFormats.at(mnemonic);
    const std::vector<ValidationRule> rules =
        Parser::gatherValidationRules(supportedArgFormats);

    // Check if any of the rules match
    for (const auto &rule : rules) {
        if (rule.size() != args.size()) {
            continue;
        }

        for (int i{0}; i < args.size(); i++) {
            if (rule[i] != args[i].type) {
                break;
            }
            // Found a match
            if (i == args.size() - 1) {
                return true;
            }
        }
    }

    return false;
}

auto Parser::gatherValidationRules(const std::vector<ArgFormat> &formats)
    -> std::vector<ValidationRule> {
    std::vector<ValidationRule> rules;

    for (const auto &argFormat : formats) {
        if (!util::isValidKey(argFormat, validationRules)) {
            throw std::runtime_error(
                "ArgFormat not found in the validationRules map");
        }
        rules.push_back(validationRules.at(argFormat));
    }

    return rules;
}
