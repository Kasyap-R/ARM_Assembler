#pragma once

#include "token.h"
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, Mnemonic> stringToMnemonic = {
    {"add", Mnemonic::ADD}, {"sub", Mnemonic::SUB},   {"mov", Mnemonic::MOV},
    {"j", Mnemonic::JUMP},  {"jump", Mnemonic::JUMP},
};

const std::unordered_map<std::string, Directive> stringToDirective = {
    {"global", Directive::GLOBAL},
    {"data", Directive::DATA},
    {"text", Directive::TEXT}};

const std::unordered_map<std::string, Register> stringToRegister =
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
