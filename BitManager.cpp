#include "BitManager.h"

void BitManager::updateOffset_bit(int pos) {
    chunck_number__ = pos / CHUNK_SIZE;
    chunck_offset__ = CHUNK_SIZE - (pos - chunck_number__ * CHUNK_SIZE) - 1;
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