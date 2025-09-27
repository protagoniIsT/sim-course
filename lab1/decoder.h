#ifndef DECODER_H
#define DECODER_H
#include <cstdint>

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

// opcode J-type
#define OPCODE_JTYPE 0b100111

// funct7
#define FUNCT7_ADD     0b101011
#define FUNCT7_AND     0b101101
#define FUNCT7_BDEP    0b111010
#define FUNCT7_CLS     0b111001
#define FUNCT7_SYSCALL 0b111110

void decode(int32_t insn, int32_t* dest);

#endif
