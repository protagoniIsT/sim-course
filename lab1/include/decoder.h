#pragma once

#include "defs.h"
#include "helpers.h"
#include "isa/InstructionInfo.h"

class Decoder {
public:
    Decoder()  = default;
    void decode(word_t insn, isa::DecodedInstr& di);

private:
    constexpr word_t extract(word_t word, std::size_t msb, std::size_t lsb);
};
