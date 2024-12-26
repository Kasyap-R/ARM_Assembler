#pragma once

#include "instruction.h"
#include "token.h"

#include <unordered_map>

using LabelMap = std::unordered_map<Label, int>;
class AssemblerState {

  public:
    std::vector<Token> tokens;
    std::vector<Instruction> instructions;
    LabelMap labelToAddress;
};
