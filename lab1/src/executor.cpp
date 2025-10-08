#include "executor.h"
#include <stdexcept>

void Executor::execute(const isa::DecodedInstr& di, std::array<int32_t, cpu::NUM_REGS>& regs, Memory& memory, int32_t& pc) {
    using namespace isa_detail;

    InnerOpCode iop = di.iopcode;

    switch (iop) {
        case InnerOpCode::OP_ADD: {
            regs[di.rd] = regs[di.rs] + regs[di.rt];
            break;
        }
        case InnerOpCode::OP_AND: {
            regs[di.rd] = regs[di.rs] & regs[di.rt];
            break;
        }
        case InnerOpCode::OP_BDEP: {
            regs[di.rs] = helpers::bit_deposit(regs[di.rt], regs[di.rd]);
            break;
        }
        case InnerOpCode::OP_BEQ:
        case InnerOpCode::OP_BNE: {
            int32_t target = static_cast<int32_t>(static_cast<uint32_t>(di.imm) << 2);
            bool cond = iop == InnerOpCode::OP_BEQ ? (regs[di.rs] == regs[di.rt]) : (regs[di.rs] != regs[di.rt]);
            if (cond) 
                pc += target;
            else 
                pc += 4;
            break;
        }
        case InnerOpCode::OP_CLS: {
            regs[di.rd] = helpers::count_leading_signs(regs[di.rs]);
            break;
        }
        case InnerOpCode::OP_LD: {            
            int32_t addr = regs[di.rs] + di.imm;
            if ((addr & 0b11) != 0) { 
                regs[8] = SyscallCode::EXCEPT; 
                break; 
            }
            regs[di.rt] = memory.load_word(addr);
            break;
        }
        case InnerOpCode::OP_RORI: {
            regs[di.rd] = helpers::rotate_right(regs[di.rs], di.imm);
            break;
        }
        case InnerOpCode::OP_SLTI: {
            regs[di.rt] = (regs[di.rs] < di.imm) ? 1 : 0;
            break;
        }
        case InnerOpCode::OP_SSAT: {
            regs[di.rd] = helpers::saturate_signed(regs[di.rs], di.imm);
            break;
        }
        case InnerOpCode::OP_ST: {
            int32_t addr = regs[di.rs] + di.imm;
            if ((addr & 0b11) != 0) { 
                regs[8] = SyscallCode::EXCEPT; 
                break; 
            }
            memory.store_word(addr, regs[di.rt]);
            break;
        }
        case InnerOpCode::OP_STP: {
            int32_t addr = regs[di.rs] + di.imm;
            if ((addr & 0b11) != 0) { 
                regs[8] = SyscallCode::EXCEPT; 
                break; 
            }
            memory.store_word(addr, regs[di.rt]);
            memory.store_word(addr + 4, regs[di.rd]);
            break;
        }
        case InnerOpCode::OP_J: {
            pc = (pc & 0xF0000000) | ((static_cast<uint32_t>(di.imm) << 2) & 0x0FFFFFFC);
            break;
        }
        case InnerOpCode::OP_SYSCALL: {
            break;
        }
    }
}
