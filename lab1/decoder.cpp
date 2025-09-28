#include "decoder.h"
// J BNE BEQ
void decode(int32_t insn, DecodedInsn& di) {
    int32_t opcode = (uint32_t)insn >> 26;
    di.opcode = static_cast<int32_t>(opcode);;
    switch (di.opcode) {
        // R-types
        case OPCODE_RTYPE: {
            int32_t funct6 = insn & 0b111111;
            switch (funct6) {
                case FUNCT6_ADD: // X[rd] ← X[rs] + X[rt]
                case FUNCT6_AND: // X[rd] ← X[rs] and X[rt]
                case FUNCT6_BDEP: {
                    di.rs = (insn >> 21) & MASK_5B; // rd
                    di.rt = (insn >> 16) & MASK_5B; // rs1
                    di.rd = (insn >> 11) & MASK_5B; // rs2
                    if (funct6 == FUNCT6_ADD)  di.iopcode = OP_ADD;
                    else if (funct6 == FUNCT6_AND)  di.iopcode = OP_AND;
                    else di.iopcode = OP_BDEP;
                    di.changes_pc = false;
                    // X[rd] ← bit_deposit(X[rs1], X[rs2])
                    break;
                }
                case FUNCT6_CLS: {
                    di.rd = (insn >> 21) & MASK_5B;
                    di.rs = (insn >> 16) & MASK_5B;
                    di.iopcode = OP_CLS;
                    di.changes_pc = false;
                    break;
                    // X[rd] ← count_leading_signs(X[rs])
                }
                case FUNCT6_SYSCALL: {
                    di.imm = (insn >> 6) & MASK_20B;
                    di.iopcode = OP_SYSCALL;
                    di.changes_pc = false;
                    /*
                     X8 ― system call number, 
                     X0 - X7 ― args, 
                     X0 ― result, see man syscall
                     */ 
                    break;
                }
            }
            break;
        }
        // I-types
        case OPCODE_SLTI:  { // SLTI
            di.rs = (insn >> 21) & MASK_5B;
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B));
            di.iopcode = OP_SLTI;
            di.changes_pc = false;
            break;
            // X[rt] ← (X[rs] < imm32) ? 1 : 0  
        }
        case OPCODE_ST: { // ST
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            di.iopcode = OP_ST;
            di.changes_pc = false;
            break;
            // memory[X[base] + sign_extend(offset)] ← X[rt]
        }
        case OPCODE_STP: { // STP
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B; // rt1
            di.rd = (insn >> 11) & MASK_5B; // rt2
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_11B)); // sign_extend(offset), lowest 2 bits must be zero, else misaligned access
            di.iopcode = OP_STP;
            di.changes_pc = false;
            break;
            // addr ← X[base] + sign_extend(offset)
            // memory[addr] ← X[rt1]
            // memory[addr + 4] ← X[rt2]
        }
        case OPCODE_RORI: // X[rd] ← rotate_right(X[rs], imm5) (cyclic shift)
        case OPCODE_SSAT: {
            di.rd = (insn >> 21) & MASK_5B;
            di.rs = (insn >> 16) & MASK_5B;
            di.imm = (insn >> 11) & MASK_5B;
            if (opcode == OPCODE_RORI) di.iopcode = OP_RORI;
            else di.iopcode = OP_SSAT;
            di.changes_pc = false;
            break;
            // X[rd] ← saturate_signed(X[rs], imm5)
        }
        case OPCODE_BNE: // BNE
            // target ← sign_extend(offset || 0b00)
            // cond ← (X[rs] != X[rt])
            // PC ← if (cond) PC + target else PC + 4
        case OPCODE_BEQ: { // BEQ
            di.rs = (insn >> 21) & MASK_5B;
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            if (opcode == OPCODE_BNE) di.iopcode = OP_BNE;
            else di.iopcode = OP_BEQ;
            di.changes_pc = true;
            break;
            // target ← sign_extend(offset || 0b00)
            // cond ← (X[rs] == X[rt])
            // PC ← if (cond) PC + target else PC + 4
        }
        case OPCODE_LD: { // LD
            di.rs = (insn >> 21) & MASK_5B; // base
            di.rt = (insn >> 16) & MASK_5B;
            di.imm = static_cast<int32_t>(static_cast<int16_t>(insn & MASK_16B)); // sign_extend(offset)
            di.iopcode = OP_LD;
            di.changes_pc = false;
            break;
            // X[rt] ← memory[X[base] + sign_extend(offset)]
        }
        // J-types
        case OPCODE_JTYPE: {
            di.imm = insn & MASK_26B;
            di.iopcode = OP_J;
            di.changes_pc = true;
            break;
            // PC ← PC[31:28] || instr_index || 0b00
        }
        default:
            std::cerr << "Unknown opcode: " << opcode << "\n";
            di.iopcode = OP_INVALID;
            return;
    }
}