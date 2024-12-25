#pragma once

#include "token.h"
#include <variant>
#include <vector>

using MachineArgument = std::variant<Register, Immediate, Label>;

struct MachineInstruction {
    Mnemonic mnemonic;
    std::vector<MachineArgument> arguments;
};

struct DirectiveInstruction {
    Directive directive;
    // TODO: figure out how to deal with directive arguments. Look into the
    // different types
    std::vector<std::variant<std::string, Directive>> arguments;
};

enum class InstructionType {
    MachineInstruction,
    DirectiveInstruction,
    Label,
};

struct Instruction {
    InstructionType type;
    std::variant<MachineInstruction, DirectiveInstruction, Label> args;
};
