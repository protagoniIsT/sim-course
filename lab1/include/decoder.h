#pragma once

#include "helpers.h"
#include "isa/InstructionDetails.h"
#include "isa/InstructionInfo.h"
#include <cstdint>

class Decoder {
public:
    Decoder()  = default;
    void decode(uint32_t insn, isa::DecodedInstr& di);

private:
    constexpr uint32_t extract(uint32_t word, std::size_t msb, std::size_t lsb);
};
