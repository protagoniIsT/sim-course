#include "decoder.h"
// funct
/**
 * ADD -     0b101011
 * AND -     0b101101
 * BDEP -    0b111010
 * CLS -     0b111001
 * SYSCALL - 0b111110
 */

void decode(int32_t insn, int32_t* dest) {
    int32_t opcode = insn >> 26;
    dest[0] = opcode;
    switch (opcode) {
        // R-types
        case 0b000000: {
            int32_t funct7 = insn & 0b111111;
            switch (funct7) {
                case FUNCT7_ADD:
                case FUNCT7_AND:
                case FUNCT7_BDEP: {
                    int32_t rs = (insn >> 21) & 0x1F;
                    int32_t rt = (insn >> 16) & 0x1F;
                    int32_t rd = (insn >> 11) & 0x1F;
                    dest[1] = rd;
                    dest[2] = rs;
                    dest[3] = rt;
                    break;
                }
                case FUNCT7_CLS: {
                    int32_t rd = (insn >> 11) & 0x1F;
                    int32_t rs = (insn >> 16) & 0x1F;
                    dest[1] = rd;
                    dest[2] = rs;
                    break;
                }
                case FUNCT7_SYSCALL: {
                    
                }
            }
            break;
        }
        // I-types
        case 0b010110: // SLTI
        case 0b000011: // ST
        case 0b001000: // STP
        case 0b111011: // RORI
        case 0b100100: // BNE
        case 0b000010: // BEQ
        case 0b011010: // LD
        // J-types
        case 0b100111:
    }
}