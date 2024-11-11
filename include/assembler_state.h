#pragma once

#include "token.h"

#include <unordered_set>

class AssemblerState {
  public:
    std::unordered_set<Label> labels;
};
