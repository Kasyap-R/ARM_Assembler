#include "parser.h"
#include "instruction.h"
#include "token.h"
#include <utility>

Parser::Parser() : pc{0} {};

auto Parser::parse(std::vector<Token> &tokens, AssemblerState &assemblerState)
    -> std::vector<Instruction> {
    std::vector<Instruction> instructions;
    for (auto &token : tokens) {
        // parseToken will return instruction as a rvalue, allowing for move
        // semantics (meaning ownership of the instruction is passed to the
        // instructions vector)
        // Also by calling std::move on token before passing, we are
        // transferring ownership to parseToken()
        instructions.push_back(
            this->parseToken(std::move(token), assemblerState));
    }
    return instructions;
}

auto Parser::parseToken(Token token, AssemblerState &assemblerState)
    -> Instruction {
    /*switch(token.type) {*/
    /*    case (TokenType::Mnemonic):*/
    /**/
    /*}*/
}
