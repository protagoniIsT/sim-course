#pragma once

#include <cstdint>
#include "defs.h"

namespace helpers {
    template<int Bits>
    constexpr word_t sign_extend(word_t value) {
        static_assert(Bits > 0 && Bits <= 31, "Bits must be between 1 and 31");
        const word_t mask = 1u << (Bits - 1);
        return (value ^ mask) - mask;
    }

    template<int Bits>
    constexpr word_t mask_b() {
        static_assert(Bits > 0 && Bits <= 32, "Bits must be between 1 and 32");
        return (Bits == 32) ? 0xFFFFFFFFu : ((1u << Bits) - 1u);
    }

    word_t bit_deposit(word_t src, word_t mask);

    word_t count_leading_signs(int32_t x);

    word_t rotate_right(word_t x, int32_t imm);

    int32_t saturate_signed(int32_t val, word_t nbits);
}