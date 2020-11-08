//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "PrimalClassifier.hpp"
#include "DistanceMetric.hpp"
#include <assert.h>

namespace mltk{
    /**
     * \brief Wrapper for the implementation of the K-Nearest Neighbors classifier algorithm.
     */
    template <typename T, typename Callable = distance::Euclidean< T > >
    class KNN: public PrimalClassifier< T > {
    private:
        /// Number k of neighbors to be considered
        size_t k;
        /// Function to compute the distance between two points
        Callable dist_function;

    public:
        KNN(std::shared_ptr<Data<T> > _samples, size_t _k, Callable dist_func = Callable())
        : PrimalClassifier< T >(_samples), k(_k), dist_function(dist_func) {}

        bool train() override;

        double evaluate(Point<T> p, bool raw_value=false) override;
    };
}

#endif //UFJF_MLTK_KNN_HPP
