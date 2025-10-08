#pragma once

#include <array>
#include <cstdint>
#include "cpu_defs.h"
#include "memory.h"
#include "decoder.h"
#include "executor.h"
#include "isa/InstructionInfo.h"
#include <iostream>

class CPU {
public:
    CPU(int32_t pc_, Memory* memory_);

    void attach_memory(Memory* memory_);

    void load_bin(const std::string& filename);

    bool do_cycle();

    int32_t get_reg(std::size_t index);
    void set_reg(std::size_t index, int32_t value);

    int32_t get_pc();
    void set_pc(int32_t pc_);

    void reset(int32_t pc_);
    
    void dump();
private:
    int32_t pc;
    std::array<int32_t, cpu::NUM_REGS> regs;
    Decoder decoder;
    Executor executor;
    Memory* memory;
};
