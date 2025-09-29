#include "decoder.h"

void decode(int32_t insn, DecodedInsn& di) {
    int32_t opcode = (uint32_t)insn >> 26;
    di.opcode = static_cast<int32_t>(opcode);;
    switch (di.opcode) {
        // R-types
        case OPCODE_RTYPE: {
            int32_t funct6 = insn & 0b111111;
            switch (funct6) {
                case FUNCT6_ADD:
                case FUNCT6_AND:
                case FUNCT6_BDEP: {
                    di.rs = (insn >> 21) & MASK_5B; // rd
                    di.rt = (insn >> 16) & MASK_5B; // rs1
                    di.rd = (insn >> 11) & MASK_5B; // rs2
                    if (funct6 == FUNCT6_ADD)  di.iopcode = OP_ADD;
                    else if (funct6 == FUNCT6_AND)  di.iopcode = OP_AND;
                    else di.iopcode = OP_BDEP;
                    di.changes_pc = false;
                    break;
                }
                case FUNCT6_CLS: {
                    di.rd = (insn >> 21) & MASK_5B;
                    di.rs = (insn >> 16) & MASK_5B;
                    di.iopcode = OP_CLS;
                    di.changes_pc = false;
                    break;
                }
                case FUNCT6_SYSCALL: {
                    di.imm = (insn >> 6) & MASK_20B;
                    di.iopcode = OP_SYSCALL;
                    di.changes_pc = false;
                    break;
                }
            }
            break;
        }
        // I-types
        case OPCODE_SLTI:  {
            di.rs = (insn >> 21) & MASK_5B;
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B));
            di.iopcode = OP_SLTI;
            di.changes_pc = false;
            break;
        }
        case OPCODE_ST: {
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            di.iopcode = OP_ST;
            di.changes_pc = false;
            break;
        }
        case OPCODE_STP: {
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B; // rt1
            di.rd = (insn >> 11) & MASK_5B; // rt2
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_11B)); // sign_extend(offset)
            di.iopcode = OP_STP;
            di.changes_pc = false;
            break;
        }
        case OPCODE_RORI:
        case OPCODE_SSAT: {
            di.rd = (insn >> 21) & MASK_5B;
            di.rs = (insn >> 16) & MASK_5B;
            di.imm = (insn >> 11) & MASK_5B;
            if (opcode == OPCODE_RORI) di.iopcode = OP_RORI;
            else di.iopcode = OP_SSAT;
            di.changes_pc = false;
            break;
        }
        case OPCODE_BNE:
        case OPCODE_BEQ: {
            di.rs = (insn >> 21) & MASK_5B;
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            if (opcode == OPCODE_BNE) di.iopcode = OP_BNE;
            else di.iopcode = OP_BEQ;
            di.changes_pc = true;
            break;
        }
        case OPCODE_LD: {
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            di.iopcode = OP_LD;
            di.changes_pc = false;
            break;
        }
        // J-types
        case OPCODE_JTYPE: {
            di.imm = insn & MASK_26B;
            di.iopcode = OP_J;
            di.changes_pc = true;
            break;
        }
    }
}