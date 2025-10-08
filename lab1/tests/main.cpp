#include "simulator.h"
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program.tisa> [args...]\n";
        return 1;
    }

    std::string path = argv[1];

    std::vector<int32_t> args;
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
    sim.load_program(path, args);
    sim.run();

    return 0;
}