#pragma once

#include "helpers.h"
#include "isa/InstructionDetails.h"
#include "isa/InstructionInfo.h"
#include <cstdint>

class Decoder {
public:
    Decoder()  = default;
    void decode(int32_t insn, isa::DecodedInstr& di);
};
