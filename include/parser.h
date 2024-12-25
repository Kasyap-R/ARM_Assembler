#pragma once

#include "argument_validation.h"
#include "assembler_state.h"
#include "instruction.h"
#include "token.h"
#include <span>
#include <vector>

/*
 * Goal of Parsing: Turn a vector of tokens into a vector of instructions
 * Steps:
 *    1. Go through the list of tokens line by line (each set represents an
 * instruction)
 *    2. Ensure that each instruction is valid (we know tokens are valid by this
 * point but doesn't neccesarily mean they form a valid instruction)
 *    3. Form either a Machine or Directive instruction and then add it to our
 * Instruction vector
 *    4. After processing of an instruction, increment pc by 4 (in ARM64 each
 * instruction takes up 4 bytes)
 * */

class Parser {
  private:
    int pc; // program counter - used to track the number of bytes taken up by
            // assembly so far
    auto parseInstruction(const std::vector<Token> &tokens,
                          AssemblerState &assemblerState)
        -> std::optional<Instruction>;
    auto parseMachineInstruction(const std::vector<Token> &tokens);
    auto parseDirectiveInstruction(const std::vector<Token> &tokens);
    static auto validateMnemonicArguments(Mnemonic mnemonic,
                                          const std::span<const Token> &args)
        -> bool;
    static auto gatherValidationRules(const std::vector<ArgFormat> &formats)
        -> std::vector<ValidationRule>;

  public:
    Parser();
    auto parse(std::vector<Token> &tokens, AssemblerState &assemblerState)
        -> std::vector<Instruction>;
};
