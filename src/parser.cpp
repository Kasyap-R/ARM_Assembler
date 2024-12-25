#include "parser.h"
#include "argument_validation.h"
#include "instruction.h"
#include "token.h"
#include "util.h"
#include <span>
#include <stdexcept>

Parser::Parser() : pc{0} {};

auto Parser::parse(std::vector<Token> &tokens, AssemblerState &assemblerState)
    -> std::vector<Instruction> {
    std::vector<Instruction> instructions;
    std::vector<Token> currLine;

    // Makes it easier to delimit by newline
    tokens.push_back(Token::createNewline());

    for (auto &token : tokens) {
        if (token.type == TokenType::Newline) {
            if (!currLine.empty()) {
                std::optional<Instruction> instruction =
                    this->parseInstruction(currLine, assemblerState);
            }
            currLine.clear();
        } else {
            currLine.push_back(token);
        }
    }
    return instructions;
}

auto Parser::parseInstruction(const std::vector<Token> &tokens,
                              AssemblerState &assemblerState)
    -> std::optional<Instruction> {
    // TODO: Add line nums to token for better errors
    const Token &firstToken = tokens[0];

    switch (firstToken.type) {
    case TokenType::Label: {
        if (tokens.size() > 1) {
            throw std::runtime_error("Unexpected tokens following label");
        }
        assemblerState.labelToAddress.insert(
            {std::get<Label>(firstToken.token), this->pc});
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
        std::vector<MachineArgument> machineArguments;

        for (const auto &argument : arguments) {
            // Use std::visit to safely extract and filter the type
    std::visit(
        [&machineArguments](auto&& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, Register> ||
                              std::is_same_v<T, Immediate> ||
                              std::is_same_v<T, Label>) {
                    // Push valid types into machineArguments
                    machineArguments.push_back(value);
                } else {
                    // Throw an error for invalid types
                    throw std::runtime_error(
                        "Unexpected token type in arguments");
                }
        },
        

        MachineInstruction machineInstruction = {
            std::get<Mnemonic>(tokens[0].token),
            std::vector<MachineArgument>{}};
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
        break;
    }
    };
    }

    auto Parser::validateMnemonicArguments(
        Mnemonic mnemonic, const std::span<const Token> &args) -> bool {
        if (!util::isValidKey(mnemonic, mnemonicsToFormats)) {
            throw std::runtime_error(
                "Unsupported mnemonic found while parsing");
        }

        const std::vector<ArgFormat> supportedArgFormats =
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
                if (i == args.size()) {
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
