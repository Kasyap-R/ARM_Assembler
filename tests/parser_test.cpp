#include "assembler_state.h"
#include "instruction.h"
#include "parser.h"
#include "token.h"
#include <gtest/gtest.h>

auto getParserOutput(const std::vector<Token> &tokens)
    -> std::vector<Instruction> {
    Parser parser;
    AssemblerState assemblerState;
    assemblerState.tokens = std::move(tokens);
    parser.parse(assemblerState);
    return assemblerState.instructions;
}

void validateParserOutput(const std::vector<Instruction> &parserOutput,
                          const std::vector<Instruction> expected) {
    ASSERT_EQ(expected.size(), parserOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], parserOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(ParserTest, ThreeArgMnemonic) {
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> tokens{token1, token2, token3, token4};
    std::vector<Instruction> expectedInstructions{Instruction{tokens}};

    validateParserOutput(getParserOutput(tokens), expectedInstructions);
}
