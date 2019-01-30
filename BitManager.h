#ifndef PDC_MINI_AEVOL_BITMANAGER_H
#define PDC_MINI_AEVOL_BITMANAGER_H

#include <cstdint>

constexpr u_int8_t LOG_CHUNK_SIZE = 5;
constexpr u_int8_t CHUNK_SIZE = 32; //
constexpr u_int8_t MAX_INTERESTING_SIZE = 22;

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

#define CHUNCK_NUMBER(pos) (pos >> LOG_CHUNK_SIZE) // pos / 2**LOG_CHUNK_SIZE
#define CHUNCK_OFFSET(pos) (pos - (CHUNCK_NUMBER(pos) << LOG_CHUNK_SIZE))  // pos - (CHUNCK_NUMBER(pos) * 2**LOG_CHUNK_SIZE)

/* x=target variable, y=mask */
#define BITMASK_SET(x, y) ((x) |= (y))
#define BITMASK_CLEAR(x, y) ((x) &= (~(y)))
#define BITMASK_FLIP(x, y) ((x) ^= (y))
#define BITMASK_CHECK_ALL(x, y) (((x) & (y)) == (y))   // warning: evaluates y twice
#define BITMASK_CHECK_ANY(x, y) ((x) & (y))

inline void set_bit(u_int32_t *dna, int pos) {
    BIT_SET(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

inline void clear_bit(u_int32_t *dna, int pos) {
    BIT_CLEAR(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

inline void flip_bit(u_int32_t *dna, int pos) {
    BIT_FLIP(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

inline int access_bit(const u_int32_t *dna, int pos) {
    return BIT_CHECK(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos));
}

inline void def_bit(u_int32_t *dna, int pos, bool value) {
    (value
     ? BIT_SET(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos))
     : BIT_CLEAR(*(dna + CHUNCK_NUMBER(pos)), CHUNCK_OFFSET(pos)));
}

inline u_int32_t get_chunck(const u_int32_t *dna, int pos) {
    int cn = CHUNCK_NUMBER(pos);
    int co = CHUNCK_OFFSET(pos);
    if (co >= CHUNK_SIZE - MAX_INTERESTING_SIZE) {
        u_int64_t holder = ((u_int64_t) (*(dna + cn + 1)) << CHUNK_SIZE);
        holder += (*(dna + cn)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return (u_int32_t) (holder >> co);
    }

    return *(dna + cn) >> co;
}

#endif //PDC_MINI_AEVOL_BITMANAGER_H
