#include "Dna.h"
#include "ExpManager.h"
#include "BitManager.h"
#include <math.h>

using namespace std;

Dna::Dna(const Dna &clone) {
    bm = BitManager();
    seq_ = clone.seq_;
    length_ = clone.length_;
    chunk_number_ = clone.chunk_number_;
}

Dna::Dna(int length, Threefry::Gen &rng) {
    bm = BitManager();
    length_ = 2 * length;
    chunk_number_ = (int) ceil(length_ / CHUNK_SIZE);
    seq_ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    // Generate a random genome
    for (int i = 0; i < length; i++)
        bm.def_bit(seq_, i, (bool) (rng.random(NB_BASE)));

    for (int i = 0; i < length; i++)
        bm.def_bit(seq_, i + length, (bool) bm.access_bit(seq_, i));
}

Dna::Dna(int32_t *genome, int length) {
    bm = BitManager();
    *seq_ = *genome;
    length_ = length * 2.0;
    chunk_number_ = (int) ceil(length_ / CHUNK_SIZE);
}

int Dna::length() const {
    return length_ / 2.0;
}

void Dna::save(gzFile backup_file) {
    int dna_length = length_;
    gzwrite(backup_file, &dna_length, sizeof(dna_length));
    gzwrite(backup_file, seq_, chunk_number_ * sizeof(int32_t));
}

void Dna::load(gzFile backup_file) {
    unsigned int dna_length;
    gzread(backup_file, &dna_length, sizeof(dna_length));
    gzread(backup_file, seq_, chunk_number_ * sizeof(int32_t));
}

void Dna::do_switch(int pos) {
    bm.flip_bit(seq_, pos);
}


int Dna::promoter_at(int pos) {
    int prom_dist[22];

    for (int motif_id = 0; motif_id < 22; motif_id++) {
        // Searching for the promoter
        prom_dist[motif_id] = bm.access_bit(&PROM_SEQ, motif_id) != bm.access_bit(seq_, pos + motif_id);
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
        if (bm.access_bit(seq_, pos + motif_id) == bm.access_bit(seq_, pos + 10 - motif_id))
            return false;

    return true;
}

bool Dna::shine_dal_start(int pos) {
    bool start = false;
    int k_t;

    for (int k = 0; k < 9; k++) {
        k_t = k >= 6 ? k + 4 : k;

        if (bm.access_bit(seq_, pos + k_t) == bm.access_bit(&SHINE_DAL_SEQ, k)) {
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

    for (int k = 0; k < 3; k++) {
        if (bm.access_bit(seq_, pos + k) == bm.access_bit(&PROTEIN_END, k)) {
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

    for (int i = 0; i < 3; i++) {
        if (bm.access_bit(seq_, pos + i))
            value += 1 << (CODON_SIZE - i - 1);
    }

    return value;
}