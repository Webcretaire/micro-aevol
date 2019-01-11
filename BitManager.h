//
// Created by webcretaire on 11/01/19.
//

#ifndef PDC_MINI_AEVOL_BITMANAGER_H
#define PDC_MINI_AEVOL_BITMANAGER_H

#include <cstdint>

constexpr int8_t CHUNK_SIZE = 32;

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

/* x=target variable, y=mask */
#define BITMASK_SET(x, y) ((x) |= (y))
#define BITMASK_CLEAR(x, y) ((x) &= (~(y)))
#define BITMASK_FLIP(x, y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x, y) (((x) & (y)) == (y))   // warning: evaluates y twice
#define BITMASK_CHECK_ANY(x, y) ((x) & (y))

int chunck_number__ = 0;
int chunck_offset__ = 0;

void updateOffset_bit(int pos) {
    int chunck_number__ = pos / CHUNK_SIZE;
    int chunck_offset__ = CHUNK_SIZE - 1 - (pos - chunck_number__ * CHUNK_SIZE);
}

int set_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_SET(*(dna + chunck_number__), chunck_offset__);
}

int clear_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_CLEAR(*(dna + chunck_number__), chunck_offset__);
}

int flip_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_FLIP(*(dna + chunck_number__), chunck_offset__);
}

int access_bit(const int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_CHECK(*(dna + chunck_number__), chunck_offset__);
}

#endif //PDC_MINI_AEVOL_BITMANAGER_H
