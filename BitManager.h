//
// Created by webcretaire on 11/01/19.
//

#ifndef PDC_MINI_AEVOL_BITMANAGER_H
#define PDC_MINI_AEVOL_BITMANAGER_H

#include <cstdint>

constexpr int8_t CHUNK_SIZE = 32;
constexpr int8_t MAX_INTERESTING_SIZE = 22;

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

#define CHUNCK_NUMBER(pos) (pos / CHUNK_SIZE)
#define CHUNCK_OFFSET(pos) (pos - CHUNCK_NUMBER(pos) * CHUNK_SIZE)

//#define DNA_SET_BIT(dna, pos) BIT_SET(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos))
//#define DNA_CLEAR_BIT(dna, pos) BIT_CLEAR(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos))
//#define DNA_FLIP_BIT(dna, pos) BIT_FLIP(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos))
//#define DNA_ACCESS_BIT(dna, pos) BIT_CHECK(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos))
//#define DNA_DEF_BIT(dna, pos, value) (value \
//    ? DNA_SET_BIT(dna, pos) \
//    : DNA_CLEAR_BIT(dna, pos))

/* x=target variable, y=mask */
#define BITMASK_SET(x, y) ((x) |= (y))
#define BITMASK_CLEAR(x, y) ((x) &= (~(y)))
#define BITMASK_FLIP(x, y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x, y) (((x) & (y)) == (y))   // warning: evaluates y twice
#define BITMASK_CHECK_ANY(x, y) ((x) & (y))

void set_bit(int32_t *dna, int pos);

void clear_bit(int32_t *dna, int pos);

void flip_bit(int32_t *dna, int pos);

int access_bit(const int32_t *dna, int pos);

void def_bit(int32_t *dna, int pos, bool value);

int32_t get_chunck(int32_t *dna, int pos);

#endif //PDC_MINI_AEVOL_BITMANAGER_H
