#include "simulator.h"
#include <string>
#include <gflags/gflags.h>

DEFINE_bool(debug, false, "Run simulator in debug mode (prints register file and PC states)");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("Usage: ./build/sim.out <program_binary.bin> [program args...]\n");

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc < 2) {
        std::cerr << "Usage: ./build/sim.out <program_binary.bin> [program args...]\n";
        return 1;
    }

    std::string path = argv[1];

    std::vector<word_t> args;
    args.reserve(argc - 2);
    for (int i = 2; i < argc; ++i) {
        try {
            args.push_back(std::stoi(argv[i]));
        } catch (const std::invalid_argument&) {
            std::cout << "[Error] Invalid argument: \"" << argv[i] << "\" (not a number)\n";
            return 1;
        } catch (const std::out_of_range&) {
            std::cout << "[Error] Argument out of range: \"" << argv[i] << "\"\n";
            return 1;
        }
    }

    Simulator sim;
    sim.set_debug_mode(FLAGS_debug);
    sim.load_program(path, args);
    sim.run();

    return 0;
}