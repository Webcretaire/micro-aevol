#include <stdint-gcc.h>
#include "BitManager.h"

int32_t get_chunck(const int32_t *dna, const int pos) {
    int cn = CHUNCK_NUMBER(pos);
    int co = CHUNCK_OFFSET(pos);
    if (co >= CHUNK_SIZE - MAX_INTERESTING_SIZE) {
        int64_t holder = ((int64_t) (*(dna + cn + 1)) << CHUNK_SIZE);
        holder += (*(dna + cn)) & 0b0000000000000000000000000000000011111111111111111111111111111111;

        return (int32_t) (holder >> co);
    }

    return *(dna + cn) >> co;
}