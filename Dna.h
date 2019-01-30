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

constexpr u_int8_t CODON_SIZE = 3;
constexpr u_int8_t CYCLE_SIZE = 22;

//constexpr const char *PROM_SEQ = "0101011001110010010110";
//constexpr const char *SHINE_DAL_SEQ = "011011000";
//constexpr const char *PROTEIN_END = "001"; // CODON_STOP
constexpr const u_int32_t PROM_SEQ = 0b00000000000110100100111001101010;
constexpr const u_int32_t PROM_MASK = 0b00000000001111111111111111111111;
constexpr const u_int32_t SHINE_DAL_SEQ = 0b00000000000000000000000000110110;
constexpr const u_int32_t PROTEIN_END = 0b00000000000000000000000000000100; // CODON_STOP

class ExpManager;

class Dna {

public:
    Dna() = default;

    Dna(const Dna &clone);

    Dna(int length, Threefry::Gen &rng);

    Dna(char *genome, int length);

    Dna(int length);

    ~Dna() = default;

    int length() const;

    void save(gzFile backup_file);

    void load(gzFile backup_file);

    void set(int pos, char c);

    void do_switch(int pos);

    int promoter_at(int pos);

    bool terminator_at(int pos);

    bool shine_dal_start(int pos);

    bool protein_stop(int pos);

    int codon_at(int pos);

private:

    u_int32_t *seq__;

    int length_;

    int dna_length_;

    int chunk_number_;
};
