#include "simulator.h"

bool Simulator::handle_syscall() {
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
        case SyscallCode::NONE:
            return true;
        default:
            return false;
    }
}

Simulator::Simulator(std::size_t mem_size, Mode mode_) 
    : mem(mem_size), 
      cpu(0, mem), 
      mode(mode_) {}

void Simulator::load_program(const std::string& path, const std::vector<int32_t>& a) {
    input_path = path;
    args = a;

    std::filesystem::path in_path(input_path);
    output_path = in_path.stem().string() + ".bin";

    std::ifstream fin(input_path);
    if (!fin) {
        std::cerr << "Could not open " << input_path << "\n";
        return;
    }
    std::ofstream fout(output_path, std::ios::binary);
    assemble(fin, fout);
    fin.close();
    fout.close();

    file_size = std::filesystem::file_size(output_path);

    mem.reset(); 
    cpu.load_bin(output_path);
    int32_t start_pc = mem.load_word(0);
    cpu.reset(start_pc);

    for (std::size_t i = 0; i < args.size(); ++i) {
        cpu.set_reg(i, args[i]);
    }

    cpu.set_reg(8, SyscallCode::NONE);
}


void Simulator::run() {
    while (cpu.do_cycle()) {
        if (cpu.get_pc() == file_size) {
            cpu.set_reg(8, SyscallCode::EXIT);
        }
        if (!handle_syscall()) return;
    }

    if (mode == Mode::DEBUG) {
        cpu.dump();
    }
}
