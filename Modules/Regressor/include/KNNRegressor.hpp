#ifndef UFJF_MLTK_KNNREGRESSOR_HPP
#define UFJF_MLTK_KNNREGRESSOR_HPP


#include "PrimalRegressor.hpp"
#include "DistanceMetric.hpp"
#include <assert.h>

namespace mltk{
        namespace regressor {
            /**
             * \brief Wrapper for the implementation of the K-Nearest Neighbors regression algorithm.
             */
            template<typename T, typename Callable = distance::Euclidean <T> >
            class KNNRegressor : public PrimalRegressor<T> {
            private:
                /// Number k of neighbors considered during training
                size_t k;
                /// Function to compute the distance between two points
                Callable dist_function;
            public:
                KNNRegressor(std::shared_ptr<Data < T>

                > _samples,
                size_t _k, Callable
                dist_func = Callable()
                )
                :

                PrimalRegressor<T> (_samples), k(_k), dist_function(dist_func) {}

                bool train() override;

                std::string getFormulationString() override;

                double evaluate(const Point <T> &p, bool raw_value = false) override;
            };
        }
}

#endif