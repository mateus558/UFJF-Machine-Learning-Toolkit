//
// Created by mateus558 on 29/07/19.
//

#include "PrimalPercCommittee.hpp"
#include "Perceptron.hpp"

template <typename T>
PrimalPercCommittee< T >::PrimalPercCommittee(std::shared_ptr<Data<T>> samples, size_t committee_size, PerceptronPrimal< T > *perc) {
    this->samples = samples;
    this->c_size = committee_size;
    this->perc = perc;
}

template <typename T>
bool PrimalPercCommittee< T >::train() {
    size_t i, j, k, size = this->samples->getSize(), dim = this->samples->getDim();
    size_t ont, it, ctot, steps;
    std::vector<double> gamma(this->c_size);
    double secs = 0.0, rmargin = 0.0, mmargin = 0.0, normW = 0.0;
    double min = 0.0, max = 0.0, norm = 1.0, *func;
    double q = this->perc->getQ();
    std::vector<size_t> vetorind(size);
    size_t lines = 1, l = 0, lin = 0;
    int order = this->permutation;
    double rate = 0.05;
    double sum = 0.0;
    bool flag = false;
    double similar = this->similarity;

}

template class PrimalPercCommittee<int>;
template class PrimalPercCommittee<double>;
template class PrimalPercCommittee<float>;
template class PrimalPercCommittee<int8_t>;
template class PrimalPercCommittee<char>;
template class PrimalPercCommittee<long long int>;
template class PrimalPercCommittee<short int>;
template class PrimalPercCommittee<long double>;
template class PrimalPercCommittee<unsigned char>;
template class PrimalPercCommittee<unsigned int>;
template class PrimalPercCommittee<unsigned short int>;