//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "PrimalClassifier.hpp"
#include "DistanceMetric.hpp"
#include <assert.h>

namespace mltk{
        namespace classifier {
            /**
             * \brief Wrapper for the implementation of the K-Nearest Neighbors classifier algorithm.
             */
            template<typename T, typename Callable = metrics::dist::Euclidean<T> >
            class KNNClassifier : public PrimalClassifier<T> {
            private:
                /// Number k of neighbors to be considered
                size_t k;
                /// Function to compute the metrics between two points
                Callable dist_function;

            public:
                explicit KNNClassifier(size_t _k, Callable dist_func = Callable())
                        : k(_k), dist_function(dist_func) {}

                KNNClassifier(Data<T> &_samples, size_t _k, Callable dist_func = Callable())
                        : PrimalClassifier<T>(mltk::make_data<T>(_samples)), k(_k), dist_function(dist_func) {}

                bool train() override;

                double evaluate(const Point<T> &p, bool raw_value = false) override;
            };
        }
}

#endif //UFJF_MLTK_KNN_HPP
