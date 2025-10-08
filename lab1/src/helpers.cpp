#include "helpers.h"

uint32_t helpers::bit_deposit(uint32_t src, uint32_t mask) {
    uint32_t result = 0;
    uint32_t src_bit = 1;
    uint32_t dst_bit = 1;
    while (mask) {
        if (mask & 1) {
            if (src & src_bit) result |= dst_bit;
            src_bit <<= 1;
        }
        mask >>= 1;
        dst_bit <<= 1;
    }
    return result;
}

uint32_t helpers::count_leading_signs(int32_t x) {
    int sign = (x < 0) ? 1 : 0;
    uint32_t count = 0;
    for (int i = 31; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (bit == sign) count++;
        else break;
    }
    return count;
}

uint32_t helpers::rotate_right(uint32_t x, int32_t imm) {
    imm &= 31;
    if (!imm) return x;
    return (x >> imm) | (x << ((32 - imm) & 31));
}

int32_t helpers::saturate_signed(int32_t val, uint32_t nbits) {
    int32_t minv = -(1 << (nbits - 1));
    int32_t maxv =  (1 << (nbits - 1)) - 1;
    if (val < minv) return minv;
    if (val > maxv) return maxv;
    return val;
}