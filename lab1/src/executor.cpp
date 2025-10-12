#include "executor.h"
#include "cpu.h"
#include <stdexcept>

bool Executor::handle_syscall(CPU& cpu) {
    switch (cpu.get_reg(8)) {
        case SyscallCode::EXIT:
            std::cout << "[SIM] Program exited.\n";
            return false;

        case SyscallCode::PRINT:
            std::cout << "[SIM] RES: " << cpu.get_reg(0) << std::endl;
            return true;

        case SyscallCode::EXCEPT:
            std::cout << "[SIM] Exception occurred (PC=" << cpu.get_pc() << ")\n";
            return false;
        default:
            return true;
    }
}

bool Executor::execute(const isa::DecodedInstr& di, CPU& cpu) {
    using namespace isa;

    InnerOpCode iop = di.iopcode;

    switch (iop) {
        case InnerOpCode::ADD: {
            cpu.set_reg(di.rd, cpu.get_reg(di.rs) + cpu.get_reg(di.rt));
            return true;
        }
        case InnerOpCode::AND: {
            cpu.set_reg(di.rd, cpu.get_reg(di.rs) & cpu.get_reg(di.rt));
            return true;
        }
        case InnerOpCode::BDEP: {
            cpu.set_reg(di.rs, helpers::bit_deposit(cpu.get_reg(di.rt), cpu.get_reg(di.rd)));
            return true;
        }
        case InnerOpCode::BEQ:
        case InnerOpCode::BNE: {
            int32_t target = static_cast<int32_t>(static_cast<uint32_t>(di.imm) << 2);
            bool cond = iop == InnerOpCode::BEQ ? (cpu.get_reg(di.rs) == cpu.get_reg(di.rt)) : (cpu.get_reg(di.rs) != cpu.get_reg(di.rt));
            if (cond) 
                cpu.update_pc(target);
            else 
                cpu.update_pc(sizeof(word_t));
            return true;
        }
        case InnerOpCode::CLS: {
            cpu.set_reg(di.rd, helpers::count_leading_signs(cpu.get_reg(di.rs)));
            return true;
        }
        case InnerOpCode::LD: {            
            int32_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned LD at addr=" << addr << "\n";
                return false;
            }
            cpu.set_reg(di.rt, cpu.get_mem().load_word(addr));
            return true;
        }
        case InnerOpCode::RORI: {
            cpu.set_reg(di.rd, helpers::rotate_right(cpu.get_reg(di.rs), di.imm));
            return true;
        }
        case InnerOpCode::SLTI: {
            cpu.set_reg(di.rt, (cpu.get_reg(di.rs) < di.imm) ? 1 : 0);
            return true;
        }
        case InnerOpCode::SSAT: {
            cpu.set_reg(di.rd, helpers::saturate_signed(cpu.get_reg(di.rs), di.imm));
            return true;
        }
        case InnerOpCode::ST: {
            int32_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned ST at addr=" << addr << "\n";
                return false; 
            }
            cpu.get_mem().store_word(addr, cpu.get_reg(di.rt));
            return true;
        }
        case InnerOpCode::STP: {
            int32_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned STP at addr=" << addr << "\n";
                return false;
            }
            cpu.get_mem().store_word(addr, cpu.get_reg(di.rt));
            cpu.get_mem().store_word(addr + sizeof(word_t), cpu.get_reg(di.rd));
            return true;
        }
        case InnerOpCode::J: {
            cpu.set_pc((cpu.get_pc() & 0xF0000000) | ((static_cast<uint32_t>(di.imm) << 2) & 0x0FFFFFFC));
            return true;
        }
        case InnerOpCode::SYSCALL: {
            return handle_syscall(cpu);
        }
        default: {
            std::cout << "[EXECUTOR] Error: Unknown instruction: " << static_cast<word_t>(iop) << std::endl;
            return false;
        }
    }
}
