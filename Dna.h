//
// Created by arrouan on 01/10/18.
//

#pragma once

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <vector>
#include <zlib.h>

#include "Threefry.h"

constexpr int8_t CODON_SIZE = 3;
constexpr int32_t TERMINATOR_MASK = 0b11110011110000000000000000000000;

constexpr const char *PROM_SEQ = "0101011001110010010110";
constexpr const char *SHINE_DAL_SEQ = "011011000";
constexpr const char *PROTEIN_END = "001"; // CODON_STOP

class ExpManager;

class Dna {

public:
    Dna() = default;

    Dna(const Dna &clone);

    Dna(int length, Threefry::Gen &rng);

    Dna(int32_t *genome, int length);

    ~Dna() = default;

    int length() const;

    void save(gzFile backup_file);

    void load(gzFile backup_file);

    void do_switch(int pos);

    int promoter_at(int pos);

    int terminator_at(int pos);

    bool shine_dal_start(int pos);

    bool protein_stop(int pos);

    int codon_at(int pos);

    int32_t *seq_;

    int32_t length_;

    int32_t chunk_number_;
};
