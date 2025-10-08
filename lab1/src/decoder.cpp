#include "decoder.h"

void Decoder::decode(int32_t insn, isa::DecodedInstr& di) {
    using namespace isa_detail;    
    uint32_t opcode = (uint32_t)insn >> 26;
    di.opcode = opcode;
    switch (di.opcode) {
        // R-types
        case OPCODE_RTYPE: {
            int32_t funct6 = insn & 0b111111;
            switch (funct6) {
                case FUNCT6_ADD:
                case FUNCT6_AND:
                case FUNCT6_BDEP: {
                    di.rs = (insn >> 21) & helpers::mask_b<5>(); // rd
                    di.rt = (insn >> 16) & helpers::mask_b<5>(); // rs1
                    di.rd = (insn >> 11) & helpers::mask_b<5>(); // rs2
                    if (funct6 == FUNCT6_ADD)  di.iopcode = OP_ADD;
                    else if (funct6 == FUNCT6_AND)  di.iopcode = OP_AND;
                    else di.iopcode = OP_BDEP;
                    break;
                }
                case FUNCT6_CLS: {
                    di.rd = (insn >> 21) & helpers::mask_b<5>();
                    di.rs = (insn >> 16) & helpers::mask_b<5>();
                    di.iopcode = OP_CLS;
                    break;
                }
                case FUNCT6_SYSCALL: {
                    di.imm = (insn >> 6) & helpers::mask_b<20>();
                    di.iopcode = OP_SYSCALL;
                    break;
                }
            }
            break;
        }
        // I-types
        case OPCODE_SLTI:  {
            di.rs = (insn >> 21) & helpers::mask_b<5>();
            di.rt = (insn >> 16) & helpers::mask_b<5>();
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_SLTI;
            break;
        }
        case OPCODE_ST: {
            di.rs = (insn >> 21) & helpers::mask_b<5>(); // base
            di.rt = (insn >> 16) & helpers::mask_b<5>();
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_ST;
            break;
        }
        case OPCODE_STP: {
            di.rs = (insn >> 21) & helpers::mask_b<5>(); // base
            di.rt = (insn >> 16) & helpers::mask_b<5>(); // rt1
            di.rd = (insn >> 11) & helpers::mask_b<5>(); // rt2
            di.imm = helpers::sign_extend<11>(insn & helpers::mask_b<11>());
            di.iopcode = OP_STP;
            break;
        }
        case OPCODE_RORI:
        case OPCODE_SSAT: {
            di.rd = (insn >> 21) & helpers::mask_b<5>();
            di.rs = (insn >> 16) & helpers::mask_b<5>();
            di.imm = (insn >> 11) & helpers::mask_b<5>();
            if (opcode == OPCODE_RORI) di.iopcode = OP_RORI;
            else di.iopcode = OP_SSAT;
            break;
        }
        case OPCODE_BNE:
        case OPCODE_BEQ: {
            di.rs = (insn >> 21) & helpers::mask_b<5>();
            di.rt = (insn >> 16) & helpers::mask_b<5>();
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            if (opcode == OPCODE_BNE) di.iopcode = OP_BNE;
            else di.iopcode = OP_BEQ;
            di.changes_pc = true;
            break;
        }
        case OPCODE_LD: {
            di.rs = (insn >> 21) & helpers::mask_b<5>(); // base
            di.rt = (insn >> 16) & helpers::mask_b<5>();
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_LD;
            break;
        }
        // J-types
        case OPCODE_JTYPE: {
            di.imm = insn & helpers::mask_b<26>();
            di.iopcode = OP_J;
            di.changes_pc = true;
            break;
        }
    }
}