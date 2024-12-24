#include "assembler_state.h"
#include "lexer.h"
#include "token.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <string>
#include <vector>

auto getLexerOutput(const std::string &lexerInput) -> std::vector<Token> {
    AssemblerState state;
    Lexer lexer;
    return lexer.tokenize(lexerInput, state);
}

void validateLexerOutput(std::vector<Token> &lexerOutput,
                         std::vector<Token> expected) {
    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, ThreeArgMnemonic) {
    std::string testInput = "add x1, x2, x3";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};
    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, MultipleInstructions) {
    std::string testInput = "add x1, x2, x3\nmov x1, x2";

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(Mnemonic::ADD);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);
    Token token5 = Token::createNewline();
    Token token6 = Token::createMnemonic(Mnemonic::MOV);
    Token token7 = Token::createRegister(reg1);
    Token token8 = Token::createRegister(reg2);
    std::vector<Token> expected = {token1, token2, token3, token4,
                                   token5, token6, token7, token8};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, SimpleLabels) {
    std::string testInput = "function:\nj function";

    Label label{"function"};

    Token token1 = Token::createLabel(label);
    Token token2 = Token::createNewline();
    Token token3 = Token::createMnemonic(Mnemonic::JUMP);

    std::vector<Token> expected = {token1, token2, token3, token1};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, SimpleImmediate) {
    std::string testInput = "add x1, #34, #0xF";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createImmediate(Immediate{34});
    Token token4 = Token::createImmediate(Immediate{15});

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, BinaryImmediate) {
    std::string testInput = "add x1, x2, #0b101";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createImmediate(Immediate{5}); // 0b101 in binary

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, SubtractionInstruction) {
    std::string testInput = "sub x3, x4, x5";
    Mnemonic mnemonic = Mnemonic::SUB;

    Register reg1 = Register::X3;
    Register reg2 = Register::X4;
    Register reg3 = Register::X5;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, MultipleLabels) {
    std::string testInput = "start:\nadd x1, x2, x3\nend:";
    Label label1{"start"};
    Label label2{"end"};

    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;
    Token newLine = Token::createNewline();
    Token token1 = Token::createLabel(label1);
    Token token2 = Token::createMnemonic(mnemonic);
    Token token3 = Token::createRegister(reg1);
    Token token4 = Token::createRegister(reg2);
    Token token5 = Token::createRegister(reg3);
    Token token6 = Token::createLabel(label2);

    std::vector<Token> expected = {token1, newLine, token2,  token3,
                                   token4, token5,  newLine, token6};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, InvalidImmediate) {
    std::string testInput = "add x1, x2, #invalid";
    EXPECT_THROW({ getLexerOutput(testInput); }, std::runtime_error);
}

TEST(LexerTest, InlineCommentIgnored) {
    std::string testInput = "add x1, x2, x3 // This is a comment";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, CommentIgnored) {
    std::string testInput = "//This is a comment\nadd x1, x2, x3";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}

TEST(LexerTest, WhitespaceTrimmed) {
    std::string testInput = "\t add x1, x2, #0b101   ";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createImmediate(Immediate{5}); // 0b101 in binary

    std::vector<Token> expected = {token1, token2, token3, token4};

    std::vector<Token> lexerOutput = getLexerOutput(testInput);
    validateLexerOutput(lexerOutput, expected);
}
