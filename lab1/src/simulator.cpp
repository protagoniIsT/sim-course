#include "simulator.h"

bool Simulator::handle_syscall() {
    switch (cpu.get_reg(8)) {
        case SyscallCode::EXIT:
            std::cout << "[SIM] Program exited.\n";
            return false;

        case SyscallCode::PRINT:
            std::cout << "[SIM] RES: " << cpu.get_reg(0) << std::endl;
            cpu.set_reg(8, SyscallCode::EXIT);
            return true;

        case SyscallCode::EXCEPT:
            std::cerr << "[SIM] Exception occurred (PC=" << cpu.get_pc() << ")\n";
            return false;
        default:
            return true;
    }
}

Simulator::Simulator(std::size_t mem_size, Mode mode_) 
    : mem(std::make_unique<Memory>(mem_size)), 
      cpu(0, mem.get()), 
      mode(mode_) {}

void Simulator::load_program(const std::string& path, const std::vector<int32_t>& a) {
    input_path = path;
    args = a;

    std::filesystem::path in_path(input_path);
    output_path = in_path.stem().string() + ".bin";

    std::ifstream fin(input_path);
    std::ofstream fout(output_path, std::ios::binary);
    assemble(fin, fout);
    fin.close();
    fout.close();

    mem = std::make_unique<Memory>(memory::MEM_SIZE); 
    cpu.attach_memory(mem.get());

    cpu.load_bin(output_path);

    int32_t start_pc = mem->load_word(0);

    cpu.reset(start_pc);

    for (std::size_t i = 0; i < args.size(); ++i) {
        cpu.set_reg(i, args[i]);
    }

    cpu.set_reg(8, SyscallCode::NONE);
}


void Simulator::run() {
    while (cpu.do_cycle()) {
        if (!handle_syscall()) break;
    }

    if (mode == Mode::DEBUG) {
        cpu.dump();
    }
}
