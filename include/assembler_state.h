#pragma once

#include "token.h"

#include <string>
#include <unordered_map>

class AssemblerState {
  public:
    std::unordered_map<std::string, Label> labels;
};
