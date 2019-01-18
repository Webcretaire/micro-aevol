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
#include "BitManager.h"

constexpr const int8_t CODON_SIZE = 3;
constexpr const int32_t TERMINATOR_MASK = 0b11110011110000000000000000000000;

constexpr const int32_t PROM_SEQ = 0b01010110011100100101100000000000;
constexpr const int32_t SHINE_DAL_SEQ = 0b01101100000000000000000000000000;
constexpr const int32_t PROTEIN_END = 0b00100000000000000000000000000000; // CODON_STOP

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

    bool terminator_at(int pos);

    bool shine_dal_start(int pos);

    bool protein_stop(int pos);

    int codon_at(int pos);

private:
    int32_t *seq_;

    int length_;

    int chunk_number_;

    BitManager bm;
};
