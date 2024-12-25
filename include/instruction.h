#pragma once

#include "token.h"
#include <vector>

struct Instruction {
    std::vector<Token> &tokens;
};
