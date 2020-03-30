//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "Classifier.hpp"

template< typename T, template <typename > class ClassifierT>
class OneVsAll: public Classifier< T > {
private:
    std::vector<std::vector<double> > weights;
    std::vector<double> biases;

public:
    OneVsAll(Data< T > _samples, int _verbose);

    bool train() override;

    double evaluate(Point<T> p) override;

    std::string getFormulationString() override;
};


#endif //UFJF_MLTK_ONEVSALL_HPP
