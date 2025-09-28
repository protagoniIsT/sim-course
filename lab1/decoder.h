#pragma once

#include <cstdint>
#include <iostream>

#define MASK_26B ((1u << 26) - 1u)
#define MASK_20B ((1u << 20) - 1u)
#define MASK_16B ((1u << 16) - 1u)
#define MASK_11B ((1u << 11) - 1u)
#define MASK_5B  ((1u << 5) - 1u)

// opcode R-type
#define OPCODE_RTYPE 0b000000

// opcodes I-type
#define OPCODE_SLTI 0b010110
#define OPCODE_ST   0b000011
#define OPCODE_STP  0b001000
#define OPCODE_RORI 0b111011
#define OPCODE_BNE  0b100100
#define OPCODE_BEQ  0b000010
#define OPCODE_LD   0b011010
#define OPCODE_SSAT 0b001111

// opcode J-type
#define OPCODE_JTYPE 0b100111

// funct6
#define FUNCT6_ADD     0b101011
#define FUNCT6_AND     0b101101
#define FUNCT6_BDEP    0b111010
#define FUNCT6_CLS     0b111001
#define FUNCT6_SYSCALL 0b111110

typedef enum {
    OP_INVALID = -1,
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

struct DecodedInsn {
    int32_t opcode;      
    int32_t rd;
    int32_t rs;
    int32_t rt;
    int32_t imm;
    InnerOpCode iopcode;
    bool changes_pc;
};

void decode(int32_t insn, DecodedInsn& di);
