#include "decoder.h"

constexpr uint32_t Decoder::extract(uint32_t word, std::size_t msb, std::size_t lsb) {
    const uint32_t width = msb - lsb + 1;
    const uint32_t mask = (width == 32) ? 0xFFFFFFFFu : ((1u << width) - 1u);
    return (word >> lsb) & mask;
}

void Decoder::decode(uint32_t insn, isa::DecodedInstr& di) {
    using namespace isa_detail;    
    uint32_t opcode = Decoder::extract(static_cast<uint32_t>(insn), 31, 26);
    di.opcode = opcode;
    switch (di.opcode) {
        // R-types
        case OPCODE_RTYPE: {
            const int32_t funct6 = Decoder::extract(insn, 5, 0);
            switch (funct6) {
                case FUNCT6_ADD:
                case FUNCT6_AND:
                case FUNCT6_BDEP: {
                    di.rs = Decoder::extract(insn, 25, 21); // rd
                    di.rt = Decoder::extract(insn, 20, 16); // rs1
                    di.rd = Decoder::extract(insn, 15, 11); // rs2
                    di.iopcode = isa::findByOpcode(di.opcode, funct6)->iopcode;
                    break;
                }
                case FUNCT6_CLS: {
                    di.rd = Decoder::extract(insn, 25, 21);
                    di.rs = Decoder::extract(insn, 20, 16);
                    di.iopcode = OP_CLS;
                    break;
                }
                case FUNCT6_SYSCALL: {
                    di.imm = Decoder::extract(insn, 25, 6);
                    di.iopcode = OP_SYSCALL;
                    break;
                }
            }
            break;
        }
        // I-types
        case OPCODE_SLTI:  {
            di.rs = Decoder::extract(insn, 25, 21);
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_SLTI;
            break;
        }
        case OPCODE_ST: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_ST;
            break;
        }
        case OPCODE_STP: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16); // rt1
            di.rd = Decoder::extract(insn, 15, 11); // rt2
            di.imm = helpers::sign_extend<11>(insn & helpers::mask_b<11>());
            di.iopcode = OP_STP;
            break;
        }
        case OPCODE_RORI:
        case OPCODE_SSAT: {
            di.rd = Decoder::extract(insn, 25, 21);
            di.rs = Decoder::extract(insn, 20, 16);
            di.imm = Decoder::extract(insn, 15, 11);
            di.iopcode = isa::findByOpcode(di.opcode, 0)->iopcode;
            break;
        }
        case OPCODE_BNE:
        case OPCODE_BEQ: {
            di.rs = Decoder::extract(insn, 25, 21);
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = isa::findByOpcode(di.opcode, 0)->iopcode;
            di.changes_pc = true;
            break;
        }
        case OPCODE_LD: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = OP_LD;
            break;
        }
        // J-types
        case OPCODE_JTYPE: {
            di.imm = Decoder::extract(insn, 25, 0);
            di.iopcode = OP_J;
            di.changes_pc = true;
            break;
        }
    }
}