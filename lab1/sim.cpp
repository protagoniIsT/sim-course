#include "sim.h"
#include "decoder.h"
#include "executor.h"
#include <fstream>
#include <iostream>

int32_t load_word(const uint8_t* mem, int32_t addr) {
    return (int32_t)mem[addr] |
           ((int32_t)mem[addr + 1] << 8)  |
           ((int32_t)mem[addr + 2] << 16) |
           ((int32_t)mem[addr + 3] << 24);
}

void store_word(uint8_t* mem, int32_t addr, int32_t value) {
    mem[addr] = (uint8_t)(value);
    mem[addr + 1] = (uint8_t)(value >> 8);
    mem[addr + 2] = (uint8_t)(value >> 16);
    mem[addr + 3] = (uint8_t)(value >> 24);
}

void load_bin(uint8_t* mem, const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open " << filename << "\n";
        std::exit(1);
    }

    in.read(reinterpret_cast<char*>(mem), MEM_SIZE);
    std::cout << "Read " << in.gcount() << " bytes\n";
    if (!in && !in.eof()) {
        std::cerr << "Error reading " << filename << "\n";
        std::exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }

    int32_t n = std::atoi(argv[1]);

    uint8_t mem[MEM_SIZE];
    int32_t regs[NUM_REGS]{0};
    regs[0] = n;
    regs[8] = 3;
    int32_t pc = 12;
    
    load_bin(mem, "fib.bin");
    std::cout << "Loaded binary\n";

    while (true) {
        int32_t insn = load_word(mem, pc);
        DecodedInsn di;
        decode(insn, di);
        execute(di, regs, mem, pc);

        if (!di.changes_pc) {
            pc += 4;
        }
        
        if (regs[8] == SIGTERM) {
            std::exit(SIGTERM);
        }
    }

    return 0;
}