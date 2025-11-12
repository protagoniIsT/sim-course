#include "decoder.h"

constexpr word_t Decoder::extract(word_t word, std::size_t msb, std::size_t lsb) {
    const word_t width = msb - lsb + 1;
    const word_t mask = (width == 32) ? 0xFFFFFFFFu : ((1u << width) - 1u);
    return (word >> lsb) & mask;
}

void Decoder::decode(word_t insn, isa::DecodedInstr& di) {
    using namespace isa;    
    word_t opcode = Decoder::extract(insn, 31, 26);
    di.opcode = opcode;
    switch (di.opcode) {
        // R-types
        case Opcode::RTYPE: {
            const int32_t funct6 = Decoder::extract(insn, 5, 0);
            switch (funct6) {
                case Funct::ADD:
                case Funct::AND:
                case Funct::BDEP: {
                    di.rs = Decoder::extract(insn, 25, 21); // rd
                    di.rt = Decoder::extract(insn, 20, 16); // rs1
                    di.rd = Decoder::extract(insn, 15, 11); // rs2
                    di.iopcode = isa::findByOpcode(di.opcode, funct6)->iopcode;
                    break;
                }
                case Funct::CLS: {
                    di.rd = Decoder::extract(insn, 25, 21);
                    di.rs = Decoder::extract(insn, 20, 16);
                    di.iopcode = InnerOpCode::CLS;
                    break;
                }
                case Funct::SYSCALL: {
                    di.imm = Decoder::extract(insn, 25, 6);
                    di.iopcode = InnerOpCode::SYSCALL;
                    break;
                }
            }
            break;
        }
        // I-types
        case Opcode::SLTI:  {
            di.rs = Decoder::extract(insn, 25, 21);
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = InnerOpCode::SLTI;
            break;
        }
        case Opcode::ST: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = InnerOpCode::ST;
            break;
        }
        case Opcode::STP: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16); // rt1
            di.rd = Decoder::extract(insn, 15, 11); // rt2
            di.imm = helpers::sign_extend<11>(insn & helpers::mask_b<11>());
            di.iopcode = InnerOpCode::STP;
            break;
        }
        case Opcode::RORI:
        case Opcode::SSAT: {
            di.rd = Decoder::extract(insn, 25, 21);
            di.rs = Decoder::extract(insn, 20, 16);
            di.imm = Decoder::extract(insn, 15, 11);
            di.iopcode = isa::findByOpcode(di.opcode, 0)->iopcode;
            break;
        }
        case Opcode::BNE:
        case Opcode::BEQ: {
            di.rs = Decoder::extract(insn, 25, 21);
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = isa::findByOpcode(di.opcode, 0)->iopcode;
            di.changes_pc = true;
            break;
        }
        case Opcode::LD: {
            di.rs = Decoder::extract(insn, 25, 21); // base
            di.rt = Decoder::extract(insn, 20, 16);
            di.imm = helpers::sign_extend<16>(insn & helpers::mask_b<16>());
            di.iopcode = InnerOpCode::LD;
            break;
        }
        // J-types
        case Opcode::JTYPE: {
            di.imm = Decoder::extract(insn, 25, 0);
            di.iopcode = InnerOpCode::J;
            di.changes_pc = true;
            break;
        }
        default: {
            di.iopcode = InnerOpCode::UNKNOWN;
            break;
        }
    }
}