//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_CLUSTERER_HPP
#define UFJF_MLTK_CLUSTERER_HPP

#include <functional>

#include "Learner.hpp"

template < typename T >
class Clusterer: public Learner< T > {
protected:
    std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >) > dist_function;
    size_t n_clusters;

};


#endif //UFJF_MLTK_CLUSTERER_HPP
