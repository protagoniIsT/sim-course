#pragma once

#include <array>
#include <cstdint>
#include "defs.h"
#include "memory.h"
#include "decoder.h"
#include "executor.h"

class CPU {
public:
    CPU(word_t pc_, Executor& executor_, Memory& memory_);

    ExecStatus do_cycle();

    word_t get_reg(std::size_t index) const;
    void set_reg(std::size_t index, word_t value);

    word_t get_pc() const;
    void set_pc(word_t pc_);
    void update_pc(word_t offset);

    Memory& get_mem();
    Executor& get_executor();

    void reset(word_t pc_);

private:
    word_t pc;
    std::array<word_t, cpu::NUM_REGS> regs;
    Decoder decoder;
    Executor& executor;
    Memory& memory;
};
