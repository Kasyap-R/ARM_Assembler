#pragma once

#include "token.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

class AssemblerState {
  public:
    std::unordered_set<Label> labels;
    std::unordered_map<Label, std::string> labelToAddress;
};
