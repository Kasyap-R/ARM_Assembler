#pragma once

#include "token.h"
#include <vector>

struct Instruction {
    std::vector<Token> tokens;
};
inline bool operator==(const Instruction &lhs, const Instruction &rhs) {
    return lhs.tokens == rhs.tokens;
}
