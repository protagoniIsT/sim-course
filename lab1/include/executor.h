#pragma once

#include <array>
#include "helpers.h"
#include "isa/InstructionInfo.h"
#include "cpu_defs.h"
#include "memory.h"

class Executor {
public:
    Executor() = default;

    void execute(const isa::DecodedInstr& di, std::array<int32_t, cpu::NUM_REGS>& regs, Memory* memory, int32_t& pc);

private:
    static uint32_t bit_deposit(uint32_t src, uint32_t mask);
    static uint32_t count_leading_signs(int32_t x);
    static uint32_t rotate_right(uint32_t x, int32_t imm);
    static int32_t saturate_signed(int32_t val, uint32_t nbits);
};
