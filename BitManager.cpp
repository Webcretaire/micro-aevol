#include <stdint-gcc.h>
#include "BitManager.h"

void BitManager::updateOffset_bit(int pos) {
    chunck_number__ = pos / CHUNK_SIZE;
    chunck_offset__ = pos - chunck_number__ * CHUNK_SIZE;
}

int BitManager::set_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_SET(*(dna + chunck_number__), chunck_offset__);
}

int BitManager::clear_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_CLEAR(*(dna + chunck_number__), chunck_offset__);
}

int BitManager::flip_bit(int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_FLIP(*(dna + chunck_number__), chunck_offset__);
}

int BitManager::access_bit(const int32_t *dna, int pos) {
    updateOffset_bit(pos);
    return BIT_CHECK(*(dna + chunck_number__), chunck_offset__);
}

int BitManager::def_bit(int32_t *dna, int pos, bool value) {
    return value ? set_bit(dna, pos) : clear_bit(dna, pos);
}

int32_t BitManager::get_chunck(int32_t *dna, int pos) {
    updateOffset_bit(pos);

    if (chunck_offset__ >= CHUNK_SIZE - MAX_INTERESTING_SIZE) {
        int64_t holder = ((int64_t) (*(dna + chunck_number__ + 1)) << CHUNK_SIZE);
        holder += (*(dna + chunck_number__)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return (int32_t) (holder >> chunck_offset__);
    }

    return *(dna + chunck_number__) >> chunck_offset__;
}

int BitManager::access_bit(const int32_t &dna, int pos) {
    return BIT_CHECK(dna, pos);
}
