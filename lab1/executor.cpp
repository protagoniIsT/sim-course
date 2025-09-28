#include "executor.h"
#include "sim.h"
#include <stdexcept>

uint32_t bit_deposit(uint32_t src, uint32_t mask) {
    uint32_t result = 0;
    uint32_t src_bit = 1;
    uint32_t dst_bit = 1;
    while (mask) {
        if (mask & 1) {
            if (src & src_bit) result |= dst_bit;
            src_bit <<= 1;
        }
        mask >>= 1;
        dst_bit <<= 1;
    }
    return result;
}

uint32_t count_leading_signs(int32_t x) {
    int sign = (x < 0) ? 1 : 0;
    uint32_t count = 0;
    for (int i = 31; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (bit == sign) count++;
        else break;
    }
    return count;
}

static inline uint32_t rotate_right(uint32_t x, int32_t imm) {
    return (x >> imm) | (x << (32 - imm));
}

int32_t saturate_signed(int32_t val, uint32_t nbits) {
    int32_t minv = -(1 << (nbits - 1));
    int32_t maxv =  (1 << (nbits - 1)) - 1;
    if (val < minv) return minv;
    if (val > maxv) return maxv;
    return val;
}

void execute(const DecodedInsn& di, int32_t* regs, uint8_t* mem, int32_t& pc) {
    InnerOpCode iop = di.iopcode;
    switch (iop) {
        case OP_ADD: {
            regs[di.rd] = regs[di.rs] + regs[di.rt];
            break;
        }
        case OP_AND: {
            regs[di.rd] = regs[di.rs] & regs[di.rt];
            break;
        }
        case OP_BDEP: {
            regs[di.rs] = bit_deposit(regs[di.rt], regs[di.rd]);
            break;
        }
        case OP_BEQ:
        case OP_BNE: {
            int32_t target = static_cast<int32_t>(static_cast<int16_t>(di.imm << 2));
            bool cond = iop == OP_BEQ ? (regs[di.rs] == regs[di.rt]) : (regs[di.rs] != regs[di.rt]);
            if (cond) 
                pc += target;
            else 
                pc += 4;
            break;
        }
        case OP_CLS: {
            regs[di.rd] = count_leading_signs(regs[di.rs]);
            break;
        }
        case OP_LD: {
            regs[di.rt] = load_word(mem, regs[di.rs] + di.imm);
            break;
        }
        case OP_RORI: {
            regs[di.rd] = rotate_right(regs[di.rs], di.imm);
            break;
        }
        case OP_SLTI: {
            regs[di.rt] = (regs[di.rs] < static_cast<int32_t>(static_cast<int16_t>(di.imm))) ? 1 : 0;
            break;
        }
        case OP_SSAT: {
            regs[di.rd] = saturate_signed(regs[di.rs], di.imm);
            break;
        }
        case OP_ST: {
            store_word(mem, regs[di.rs] + di.imm, regs[di.rt]);
            break;
        }
        case OP_STP: {
            if ((di.imm & 0b11) != 0) {
                regs[8] = SIGEXCEPT;
                break;
            }
            int32_t addr = regs[di.rs] + di.imm;
            store_word(mem, addr, regs[di.rt]);
            store_word(mem, addr + 4, regs[di.rd]);
            break;
        }
        case OP_J: {
            pc = (pc & 0xF0000000) | ((di.imm << 2) & 0x0FFFFFFC);
            break;
        }
        case OP_SYSCALL: {
            switch(regs[8]) {
                case SIGPRINT: {
                    std::cout << regs[0] << std::endl;
                    break;
                }
                case SIGEXCEPT: {
                    std::cout << "Misaligned STP offset\n";
                    break;
                }
                default:
                    std::cout << "Unknown syscall: " << regs[8] << "\n";
                    break;
            }
            regs[8] = SIGTERM;
            break;
        }
    }
}