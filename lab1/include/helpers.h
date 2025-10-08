#pragma once

#include <cstdint>

namespace helpers {
    template<int Bits>
    constexpr int32_t sign_extend(uint32_t value) {
        static_assert(Bits > 0 && Bits <= 31, "Bits must be between 1 and 31");
        const uint32_t mask = 1u << (Bits - 1);
        return (value ^ mask) - mask;
    }

    template<int Bits>
    constexpr uint32_t mask_b() {
        static_assert(Bits > 0 && Bits <= 32, "Bits must be between 1 and 32");
        return (Bits == 32) ? 0xFFFFFFFFu : ((1u << Bits) - 1u);
    }

    uint32_t bit_deposit(uint32_t src, uint32_t mask);

    uint32_t count_leading_signs(int32_t x);

    uint32_t rotate_right(uint32_t x, int32_t imm);

    int32_t saturate_signed(int32_t val, uint32_t nbits);
}