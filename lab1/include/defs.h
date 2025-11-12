#pragma once

#include <cstdint>

namespace cpu {
    inline constexpr int NUM_REGS = 32;
}

namespace memory {
    constexpr int MEM_SIZE = 2048;
}

using word_t = uint32_t;
