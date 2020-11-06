//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_CLUSTERER_HPP
#define UFJF_MLTK_CLUSTERER_HPP

#include "DistanceMetric.hpp"
#include "../../Core/include/Learner.hpp"

template < typename T, typename Callable = EuclideanDistance< T > >
class Clusterer: public Learner< T > {
protected:
    Callable dist_function;
    size_t n_clusters;
    std::vector<std::vector<T> > centers;
    std::vector<std::vector<size_t> > clusters;

public:
    Clusterer() {}

    Clusterer(DataPointer< T > samples = nullptr, size_t clusters = 0)
    : Learner< T > (samples), n_clusters(clusters) {}
};


#endif //UFJF_MLTK_CLUSTERER_HPP
