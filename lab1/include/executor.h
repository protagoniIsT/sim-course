#pragma once

#include <array>
#include "defs.h"
#include "helpers.h"
#include "isa/InstructionInfo.h"
#include "memory.h"

class CPU;

enum class ExecStatus {
    CONTINUE,
    EXITED,
    EXCEPT
};

class Executor {
public:
    Executor() = default;

    ExecStatus execute(const isa::DecodedInstr& di, CPU& cpu);
    
    ExecStatus handle_syscall(CPU& cpu);

private:
    static word_t bit_deposit(word_t src, word_t mask);
    static word_t count_leading_signs(int32_t x);
    static word_t rotate_right(word_t x, int32_t imm);
    static int32_t saturate_signed(int32_t val, word_t nbits);
};
