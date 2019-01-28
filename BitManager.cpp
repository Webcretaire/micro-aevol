#include <stdint-gcc.h>
#include "BitManager.h"

void set_bit(int32_t *dna, int pos) {
    BIT_SET(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

void clear_bit(int32_t *dna, int pos) {
    BIT_CLEAR(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

void flip_bit(int32_t *dna, int pos) {
    BIT_FLIP(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

int access_bit(const int32_t *dna, int pos) {
    return BIT_CHECK(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

void def_bit(int32_t *dna, int pos, bool value) {
    return value ? set_bit(dna, pos) : clear_bit(dna, pos);
}

int32_t get_chunck(int32_t *dna, int pos) {
    int cn = CHUNCK_NUMBER(pos);
    int co = CHUNCK_OFFSET(pos);
    if (co >= CHUNK_SIZE - MAX_INTERESTING_SIZE) {
        int64_t holder = ((int64_t) (*(dna + cn + 1)) << CHUNK_SIZE);
        holder += (*(dna + cn)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return (int32_t) (holder >> co);
    }

    return *(dna + cn) >> co;
}

int access_bit(const int32_t &dna, int pos) {
    return BIT_CHECK(dna, pos);
}
