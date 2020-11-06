//
// Created by mateus558 on 26/03/2020.
//

#ifndef UFJF_MLTK_KMEANS_HPP
#define UFJF_MLTK_KMEANS_HPP

#include "Clusterer.hpp"

template<typename T, typename Callable = EuclideanDistance< T > >
class KMeans: public Clusterer< T > {
private:
    std::string initialization;

public:
    KMeans(std::shared_ptr<Data<T> > _samples, size_t k, const std::string& initialization = "random");

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;

    std::string getFormulationString() override;

};


#endif //UFJF_MLTK_KMEANS_HPP
