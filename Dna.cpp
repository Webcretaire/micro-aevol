#include "Dna.h"
#include "ExpManager.h"
#include "BitManager.h"
#include <math.h>

Dna::Dna(const Dna &clone) {
    bm = BitManager();
    length_ = clone.length_;
    chunk_number_ = clone.chunk_number_;
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    for (int i = 0; i < length_; i++)
        bm.def_bit(seq__, i, (bool) bm.access_bit(clone.seq__, i));
}

Dna::Dna(int length, Threefry::Gen &rng) {
    bm = BitManager();

    length_ = length;
    chunk_number_ = (int) ceil(length_ / CHUNK_SIZE);
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    // Copy
    for (int i = 0; i < length; i++)
        bm.def_bit(seq__, i, (bool) rng.random(NB_BASE));
}

Dna::Dna(char *genome, int length) {
    bm = BitManager();
    length_ = length;
    chunk_number_ = (int) ceil(length_ / CHUNK_SIZE);
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    // Copy
    for (int i = 0; i < length; i++)
        bm.def_bit(seq__, i, genome[i] == '1');
}

Dna::Dna(int length) {
    bm = BitManager();

    length_ = length;
    chunk_number_ = (int) ceil(length_ / CHUNK_SIZE);
    seq__ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
}

int Dna::length() const {
    return length_;
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
}


int Dna::promoter_at(int pos) {
    int prom_dist[22];

    for (int motif_id = 0; motif_id < 22; motif_id++) {
        // Searching for the promoter
        prom_dist[motif_id] =
                bm.access_bit(&PROM_SEQ, motif_id)
                != bm.access_bit(seq__, pos + motif_id >= length_ ? pos + motif_id - length_ : pos + motif_id);
    }


    // Computing if a promoter exists at that position
    int dist_lead = prom_dist[0] +
                    prom_dist[1] +
                    prom_dist[2] +
                    prom_dist[3] +
                    prom_dist[4] +
                    prom_dist[5] +
                    prom_dist[6] +
                    prom_dist[7] +
                    prom_dist[8] +
                    prom_dist[9] +
                    prom_dist[10] +
                    prom_dist[11] +
                    prom_dist[12] +
                    prom_dist[13] +
                    prom_dist[14] +
                    prom_dist[15] +
                    prom_dist[16] +
                    prom_dist[17] +
                    prom_dist[18] +
                    prom_dist[19] +
                    prom_dist[20] +
                    prom_dist[21];

    return dist_lead;
}

bool Dna::terminator_at(int pos) {
    for (int motif_id = 0; motif_id < 4; motif_id++)
        if (bm.access_bit(seq__, pos + motif_id >= length_ ? pos + motif_id - length_ : pos + motif_id)
            ==
            bm.access_bit(seq__, pos - motif_id + 10 >= length_ ? pos - motif_id + 10 - length_ : pos - motif_id + 10))
            return false;

    return true;
}

bool Dna::shine_dal_start(int pos) {
    bool start = false;
    int t_pos, k_t;

    for (int k = 0; k < 9; k++) {
        k_t = k >= 6 ? k + 4 : k;
        t_pos = pos + k_t >= length_
                ? pos + k_t - length_
                : pos + k_t;

        if (bm.access_bit(seq__, t_pos) == bm.access_bit(&SHINE_DAL_SEQ, k)) {
            start = true;
        } else {
            start = false;
            break;
        }
    }

    return start;
}

bool Dna::protein_stop(int pos) {
    bool is_protein;
    int t_k;

    for (int k = 0; k < 3; k++) {
        t_k = pos + k >= length_ ?
              pos - length_ + k :
              pos + k;

        if (bm.access_bit(seq__, t_k) ==
            bm.access_bit(&PROTEIN_END, k)) {
            is_protein = true;
        } else {
            is_protein = false;
            break;
        }
    }

    return is_protein;
}

int Dna::codon_at(int pos) {
    int value = 0;

    int t_pos;

    for (int i = 0; i < 3; i++) {
        t_pos =
                pos + i >= length_ ? pos + i -
                                     length_
                                   : pos + i;
        if (bm.access_bit(seq__, t_pos))
            value += 1 << (CODON_SIZE - i - 1);
    }

    return value;
}