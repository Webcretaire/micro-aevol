#include "Dna.h"
#include "ExpManager.h"
#include "BitManager.h"

Dna::Dna(const Dna &clone) {
    bm = BitManager();
    length_ = clone.length_;
    dna_length_ = clone.dna_length_;
    chunk_number_ = clone.chunk_number_;
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    for (int i = 0; i < chunk_number_; i++)
        seq__[i] = clone.seq__[i];
}

Dna::Dna(int length, Threefry::Gen &rng) {
    bm = BitManager();
    length_ = length + CYCLE_SIZE;
    dna_length_ = length;
    chunk_number_ = (length_ + CHUNK_SIZE - 1) / CHUNK_SIZE;
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));

    for (int i = 0; i < length; i++)
        bm.def_bit(seq__, i, (bool) rng.random(NB_BASE));

    // Cyclic DNA
    for (int i = 0; i < CYCLE_SIZE; i++)
        bm.def_bit(seq__, i + length, (bool) bm.access_bit(*seq__, i));
}

Dna::Dna(char *genome, int length) {
    bm = BitManager();
    length_ = length + CYCLE_SIZE;
    dna_length_ = length;
    chunk_number_ = (length_ + CHUNK_SIZE - 1) / CHUNK_SIZE;
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    // Copy
    for (int i = 0; i < length; i++)
        bm.def_bit(seq__, i, genome[i] == '1');

    // Cyclic DNA
    for (int i = 0; i < CYCLE_SIZE; i++)
        bm.def_bit(seq__, i + length, (bool) bm.access_bit(*seq__, i));
}

Dna::Dna(int length) {
    bm = BitManager();
    length_ = length + CYCLE_SIZE;
    dna_length_ = length;
    chunk_number_ = (length_ + CHUNK_SIZE - 1) / CHUNK_SIZE;
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
}

int Dna::length() const {
    return dna_length_;
}

void Dna::save(gzFile backup_file) {
    int dna_length = length_;
    gzwrite(backup_file, &dna_length, sizeof(dna_length));
    gzwrite(backup_file, seq__, chunk_number_ * sizeof(int32_t));
}

void Dna::load(gzFile backup_file) {
    int dna_length;
    gzread(backup_file, &dna_length, sizeof(dna_length));
    gzread(backup_file, seq__, chunk_number_ * sizeof(int32_t));
}

void Dna::set(int pos, char c) {
    bm.def_bit(seq__, pos, c == '1');
}

void Dna::do_switch(int pos) {
    bm.flip_bit(seq__, pos);
    if (pos < CYCLE_SIZE)
        bm.flip_bit(seq__, dna_length_ + pos);
}

int Dna::promoter_at(int pos) {
    return __builtin_popcount((bm.get_chunck(seq__, pos) & PROM_MASK) ^ PROM_SEQ);
}

bool Dna::terminator_at(int pos) {
    for (int motif_id = 0; motif_id < 4; motif_id++)
        if (bm.access_bit(seq__, pos + motif_id) == bm.access_bit(seq__, pos - motif_id + 10))
            return false;

    return true;
}

bool Dna::shine_dal_start(int pos) {
    for (int k = 0; k < 6; k++)
        if (bm.access_bit(seq__, pos + k) != bm.access_bit(SHINE_DAL_SEQ, k))
            return false;

    for (int k = 10; k < 13; k++)
        if (bm.access_bit(seq__, pos + k) != bm.access_bit(SHINE_DAL_SEQ, k))
            return false;

    return true;
}

bool Dna::protein_stop(int pos) {
    for (int k = 0; k < 3; k++)
        if (bm.access_bit(seq__, pos + k) != bm.access_bit(PROTEIN_END, k))
            return false;

    return true;
}

int Dna::codon_at(int pos) {
    int value = 0;

    for (int i = 0; i < 3; i++)
        if (bm.access_bit(seq__, pos + i))
            value += 1 << (CODON_SIZE - i - 1);


    return value;
}