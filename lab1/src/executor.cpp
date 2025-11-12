#include "executor.h"
#include "cpu.h"

ExecStatus Executor::handle_syscall(CPU& cpu) {
    switch (cpu.get_reg(8)) {
        case SyscallCode::EXIT:
            std::cout << "[SIM] Program exited.\n";
            return ExecStatus::EXITED;

        case SyscallCode::PRINT:
            std::cout << "[SIM] RES: " << cpu.get_reg(0) << std::endl;
            return ExecStatus::CONTINUE;

        case SyscallCode::EXCEPT:
            std::cout << "[SIM] Exception occurred (PC=" << cpu.get_pc() << ")\n";
            return ExecStatus::EXCEPT;

        default:
            std::cerr << "[SIM] Invalid syscall code: " << cpu.get_reg(8) << "\n";
            return ExecStatus::EXCEPT;
    }
}

ExecStatus Executor::execute(const isa::DecodedInstr& di, CPU& cpu) {
    using namespace isa;

    InnerOpCode iop = di.iopcode;

    switch (iop) {
        case InnerOpCode::ADD: {
            cpu.set_reg(di.rd, cpu.get_reg(di.rs) + cpu.get_reg(di.rt));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::AND: {
            cpu.set_reg(di.rd, cpu.get_reg(di.rs) & cpu.get_reg(di.rt));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::BDEP: {
            cpu.set_reg(di.rs, helpers::bit_deposit(cpu.get_reg(di.rt), cpu.get_reg(di.rd)));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::BEQ:
        case InnerOpCode::BNE: {
            bool cond = iop == InnerOpCode::BEQ ? (cpu.get_reg(di.rs) == cpu.get_reg(di.rt)) : (cpu.get_reg(di.rs) != cpu.get_reg(di.rt));
            if (cond) {
                word_t target = di.imm << 2;
                cpu.update_pc(target);
            } else {
                cpu.update_pc(sizeof(word_t));
            }
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::CLS: {
            cpu.set_reg(di.rd, helpers::count_leading_signs(static_cast<int32_t>(cpu.get_reg(di.rs))));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::LD: {            
            word_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned LD at addr=" << addr << "\n";
                return ExecStatus::EXCEPT;
            }
            cpu.set_reg(di.rt, cpu.get_mem().load_word(addr, AccessType::READ));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::RORI: {
            cpu.set_reg(di.rd, helpers::rotate_right(cpu.get_reg(di.rs), di.imm));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::SLTI: {
            cpu.set_reg(di.rt, cpu.get_reg(di.rs) < di.imm ? 1u : 0u);
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::SSAT: {
            cpu.set_reg(di.rd, helpers::saturate_signed(static_cast<int32_t>(cpu.get_reg(di.rs)), di.imm));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::ST: {
            word_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned ST at addr=" << addr << "\n";
                return ExecStatus::EXCEPT; 
            }
            cpu.get_mem().store_word(addr, cpu.get_reg(di.rt));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::STP: {
            word_t addr = cpu.get_reg(di.rs) + di.imm;
            if ((addr & 0b11) != 0) { 
                cpu.set_reg(8, SyscallCode::EXCEPT);
                std::cerr << "[EXCEPT] Misaligned STP at addr=" << addr << "\n";
                return ExecStatus::EXCEPT;
            }
            cpu.get_mem().store_word(addr, cpu.get_reg(di.rt));
            cpu.get_mem().store_word(addr + sizeof(word_t), cpu.get_reg(di.rd));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::J: {
            cpu.set_pc((cpu.get_pc() & 0xF0000000) | ((static_cast<uint32_t>(di.imm) << 2) & 0x0FFFFFFC));
            return ExecStatus::CONTINUE;
        }
        case InnerOpCode::SYSCALL: {
            return handle_syscall(cpu);
        }
        default: {
            cpu.set_reg(8, SyscallCode::EXCEPT);
            std::cerr << "[EXECUTOR] Error: Unknown instruction (InnerOpCode): " << static_cast<word_t>(iop) << " at PC = " << cpu.get_pc() << std::endl;
            return ExecStatus::EXCEPT;
        }
    }
}
