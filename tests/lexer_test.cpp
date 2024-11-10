#include "lexer.h"
#include "token.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <string>
#include <vector>

TEST(LexerTest, ThreeArgMnemonic) {
    std::string test_input = "add x1, x2, x3";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, MultipleInstructions) {
    std::string test_input = "add x1, x2, x3\nmov x1, x2";

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(Mnemonic::ADD);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    Token token5 = Token::createMnemonic(Mnemonic::MOV);
    Token token6 = Token::createRegister(reg1);
    Token token7 = Token::createRegister(reg2);
    std::vector<Token> expected = {token1, token2, token3, token4,
                                   token5, token6, token7};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, SimpleLabels) {
    std::string test_input = "function:\nj function";

    Label label{"function"};

    Token token1 = Token::createLabel(label);
    Token token2 = Token::createMnemonic(Mnemonic::JUMP);

    std::vector<Token> expected = {token1, token2, token1};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, SimpleImmediate) {
    std::string test_input = "add x1, #34, #0xF";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createImmediate(Immediate{34});
    Token token4 = Token::createImmediate(Immediate{15});

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, BinaryImmediate) {
    std::string test_input = "add x1, x2, #0b101";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createImmediate(Immediate{5}); // 0b101 in binary

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, SubtractionInstruction) {
    std::string test_input = "sub x3, x4, x5";
    Mnemonic mnemonic = Mnemonic::SUB;

    Register reg1 = Register::X3;
    Register reg2 = Register::X4;
    Register reg3 = Register::X5;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, MultipleLabels) {
    std::string test_input = "start:\nadd x1, x2, x3\nend:";
    Label label1{"start"};
    Label label2{"end"};

    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createLabel(label1);
    Token token2 = Token::createMnemonic(mnemonic);
    Token token3 = Token::createRegister(reg1);
    Token token4 = Token::createRegister(reg2);
    Token token5 = Token::createRegister(reg3);
    Token token6 = Token::createLabel(label2);

    std::vector<Token> expected = {token1, token2, token3,
                                   token4, token5, token6};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, InvalidImmediate) {
    std::string test_input = "add x1, x2, #invalid";

    Lexer lexer(test_input);

    EXPECT_THROW({ lexer.tokenize(); }, std::runtime_error);
}

TEST(LexerTest, InlineCommentIgnored) {
    std::string test_input = "add x1, x2, x3 // This is a comment";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, CommentIgnored) {
    std::string test_input = "//This is a comment\nadd x1, x2, x3";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;
    Register reg3 = Register::X3;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createRegister(reg3);

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}

TEST(LexerTest, WhitespaceTrimmed) {
    std::string test_input = "\t add x1, x2, #0b101   ";
    Mnemonic mnemonic = Mnemonic::ADD;

    Register reg1 = Register::X1;
    Register reg2 = Register::X2;

    Token token1 = Token::createMnemonic(mnemonic);
    Token token2 = Token::createRegister(reg1);
    Token token3 = Token::createRegister(reg2);
    Token token4 = Token::createImmediate(Immediate{5}); // 0b101 in binary

    std::vector<Token> expected = {token1, token2, token3, token4};

    Lexer lexer(test_input);
    std::vector<Token> lexerOutput = lexer.tokenize();

    ASSERT_EQ(expected.size(), lexerOutput.size())
        << "Length of lexer token list and expected token list don't match\n";

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i], lexerOutput[i])
            << "Didn't match at index: " + std::to_string(i) << "\n";
    }
}
