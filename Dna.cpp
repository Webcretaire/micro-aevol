//
// Created by arrouan on 01/10/18.
//

#include "Dna.h"
#include "ExpManager.h"
#include "BitManager.h"
#include <math.h>

using namespace std;

Dna::Dna(const Dna &clone) : seq_(clone.seq_) {
}

Dna::Dna(int length, Threefry::Gen &rng) {
    length_ = 2 * length;
    chunk_number_ = ceil(length * 2 / CHUNK_SIZE);
    seq_ = (int32_t *) malloc(chunk_number_ * sizeof(int32_t));
    // Generate a random genome
    for (int32_t i = 0; i < chunk_number_ / 2; i++)
        seq_[i] = rng.random(NB_BASE);

    for (int32_t i = 0; i < chunk_number_ / 2; i++)
        seq_[i + chunk_number_ / 2] = seq_[i];
}

Dna::Dna(int32_t *genome, int length) {
    *seq_ = *genome;
    length_ = length;
}

int Dna::length() const {
    return length_;
}

void Dna::save(gzFile backup_file) {
    int dna_length = length();
    gzwrite(backup_file, &dna_length, sizeof(dna_length));
    gzwrite(backup_file, seq_, chunk_number_ * sizeof(int32_t));
}

void Dna::load(gzFile backup_file) {
    int dna_length;
    gzread(backup_file, &dna_length, sizeof(dna_length));
    gzread(backup_file, seq_, chunk_number_ * sizeof(int32_t));
}

void Dna::do_switch(int pos) {
    flip_bit(seq_, pos);
}


int Dna::promoter_at(int pos) {
    int prom_dist[22];

    for (int motif_id = 0; motif_id < 22; motif_id++) {
        // Searching for the promoter
        prom_dist[motif_id] = (PROM_SEQ[motif_id] == '0' ? 0 : 1) == access_bit(seq_, pos + motif_id) ? 0 : 1;
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

int Dna::terminator_at(int pos) {
    int term_dist[4];

    for (int motif_id = 0; motif_id < 4; motif_id++) {
        // Search for the terminators
        term_dist[motif_id] = access_bit(seq_, pos + motif_id) != access_bit(seq_, pos - motif_id + 10) ? 1 : 0;
    }

    int dist_term_lead = term_dist[0] +
                         term_dist[1] +
                         term_dist[2] +
                         term_dist[3];

    return dist_term_lead;
}

bool Dna::shine_dal_start(int pos) {
    bool start = false;
    int k_t;

    for (int k = 0; k < 9; k++) {
        k_t = k >= 6 ? k + 4 : k;

        if (access_bit(seq_, pos + k_t) ==
            (SHINE_DAL_SEQ[k] == '0' ? 0 : 1)) {
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
        if (access_bit(seq_, pos + k) ==
            (PROTEIN_END[k] == '0' ? 0 : 1)) {
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
        if (access_bit(seq_, pos + i))
            value += 1 << (CODON_SIZE - i - 1);
    }

    return value;
}