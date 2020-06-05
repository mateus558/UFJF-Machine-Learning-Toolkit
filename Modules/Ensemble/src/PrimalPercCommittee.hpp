//
// Created by mateus558 on 29/07/19.
//

#ifndef UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP
#define UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP

#include "PrimalCommittee.hpp"
#include "Perceptron.hpp"

template <typename T>
class PrimalPercCommittee: public PrimalCommittee<T> {
private:
    PerceptronPrimal< T > *perc;
public:
    explicit PrimalPercCommittee(std::shared_ptr<Data< T > > samples = nullptr, size_t committee_size= 1, PerceptronPrimal< T > *perc = nullptr);
    bool train();
};


#endif //UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP
