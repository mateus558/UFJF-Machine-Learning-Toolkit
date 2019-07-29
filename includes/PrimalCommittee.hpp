//
// Created by mateus558 on 29/07/19.
//

#ifndef UFJF_MLTK_PRIMALCOMMITTEE_HPP
#define UFJF_MLTK_PRIMALCOMMITTEE_HPP

#include "Ensemble.hpp"

template <typename T>
class PrimalCommittee: public Ensemble<T> {
protected:
    /// Weights matrix.
    std::vector<std::vector<double> > w;
    std::vector<double> bias;
    size_t permutation;
    /// similarity.
    double similarity;
public:

    double evaluate(Point< T > p) {
        return 0.0;
    }
};


#endif //UFJF_MLTK_PRIMALCOMMITTEE_HPP
