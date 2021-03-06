// ***************************************************************************************************************
//
//          Mini-Aevol is a reduced version of Aevol -- An in silico experimental evolution platform
//
// ***************************************************************************************************************
//
// Copyright: See the AUTHORS file provided with the package or <https://gitlab.inria.fr/rouzaudc/mini-aevol>
// Web: https://gitlab.inria.fr/rouzaudc/mini-aevol
// E-mail: See <jonathan.rouzaud-cornabas@inria.fr>
// Original Authors : Jonathan Rouzaud-Cornabas
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ***************************************************************************************************************


#ifndef RAEVOL_CUDA_DNAMUTATOR_H
#define RAEVOL_CUDA_DNAMUTATOR_H

#include <list>
#include <vector>

#include "Organism.h"
#include "Threefry.h"
#include "MutationEvent.h"


/**
 * Class that generates the mutation events for a given Organism
 */
class DnaMutator {
public:

    struct TypeMutation {
        MutationEventType type_;
        int32_t pos_1_ = 0, pos_2_ = 0, pos_3_ = 0;
        int32_t seq_size_ = 0;
        char seq[6];
        int32_t init_genome_len = 0;
    };

    DnaMutator(Threefry::Gen *mut_prng, int length,
               double mutation_rate,
               int indiv_id);

    ~DnaMutator() {
        int cpt = 0;
        for (auto repl : mutation_list_) {
            delete repl;
            cpt++;
        }

        mutation_list_.clear();
    }

    void generate_mutations();

    void generate_small_mutations();

    MutationEvent *generate_next_mutation(int length);

    bool mutation_available() { return ((cpt_mut_) > 0); }

    std::list<MutationEvent *> mutation_list_;

    bool hasMutate() { return hasMutate_; }

    bool setMutate(bool mutate) { hasMutate_ = mutate; }

    std::vector <TypeMutation> generate_mutation_array(int length, int *new_length);

    static char BacktraceBase(int32_t locus, int nb_events,
                              const TypeMutation *muts, const std::vector<char> ori_gen, int indiv_id);

    static int mod(int a, int b) {

        assert(b > 0);

        while (a < 0) a += b;
        while (a >= b) a -= b;

        return a;
    }

    int id_;

// private:
    Threefry::Gen *mut_prng_;
    int length_;

    double mutation_rate_;


    //--------------------------- Mutation counters
    int nb_swi_;
    int nb_mut_;

    int cpt_mut_;

    int min_genome_length_ = 10;
    int max_genome_length_ = 10000000;

    bool hasMutate_ = false;
};


#endif //RAEVOL_CUDA_DNAMUTATOR_H
