#pragma once

#include <cstdint>

namespace isa_detail {

    // opcode R-type
    constexpr int OPCODE_RTYPE = 0b000000;

    // opcodes I-type
    constexpr int OPCODE_SLTI = 0b010110;
    constexpr int OPCODE_ST = 0b000011;
    constexpr int OPCODE_STP = 0b001000;
    constexpr int OPCODE_RORI = 0b111011;
    constexpr int OPCODE_BNE = 0b100100;
    constexpr int OPCODE_BEQ = 0b000010;
    constexpr int OPCODE_LD = 0b011010;
    constexpr int OPCODE_SSAT = 0b001111;

    // opcode J-type
    constexpr int OPCODE_JTYPE = 0b100111;

    // funct6
    constexpr int FUNCT6_ADD = 0b101011;
    constexpr int FUNCT6_AND = 0b101101;
    constexpr int FUNCT6_BDEP = 0b111010;
    constexpr int FUNCT6_CLS = 0b111001;
    constexpr int FUNCT6_SYSCALL = 0b111110;

    typedef enum {
        OP_ADD,
        OP_AND,
        OP_BDEP,
        OP_CLS,
        OP_SSAT,
        OP_SYSCALL,
        OP_SLTI,
        OP_ST,
        OP_STP,
        OP_RORI,
        OP_BNE,
        OP_BEQ,
        OP_LD,
        OP_J
    } InnerOpCode;
} // end namespace isa_detail


enum SyscallCode {
    EXIT,
    PRINT,
    EXCEPT,
    NONE
};