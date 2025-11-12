#pragma once

#include "defs.h"

namespace isa {

    enum Opcode {
        // opcode R-type
        RTYPE = 0b000000,

        // opcodes I-type
        SLTI = 0b010110,
        ST = 0b000011,
        STP = 0b001000,
        RORI = 0b111011,
        BNE = 0b100100,
        BEQ = 0b000010,
        LD = 0b011010,
        SSAT = 0b001111,

        // opcode J-type
        JTYPE = 0b100111
    };


    // funct6
    enum Funct {
        ADD = 0b101011,
        AND = 0b101101,
        BDEP = 0b111010,
        CLS = 0b111001,
        SYSCALL = 0b111110
    };
     

    enum class InnerOpCode {
        ADD,
        AND,
        BDEP,
        CLS,
        SSAT,
        SYSCALL,
        SLTI,
        ST,
        STP,
        RORI,
        BNE,
        BEQ,
        LD,
        J,
        UNKNOWN
    };
} // end namespace isa


enum SyscallCode {
    EXIT,
    PRINT,
    EXCEPT,
};