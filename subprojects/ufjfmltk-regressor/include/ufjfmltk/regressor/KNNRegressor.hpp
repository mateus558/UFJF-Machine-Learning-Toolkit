#pragma once


#include "PrimalRegressor.hpp"
#include "ufjfmltk/core/DistanceMetric.hpp"
#include <cassert>

namespace mltk{
        namespace regressor {
            /**
             * \brief Wrapper for the implementation of the K-Nearest Neighbors regression algorithm.
             */
            template<typename T = double, typename Callable = metrics::dist::Euclidean <T> >
            class KNNRegressor : public PrimalRegressor<T> {
            private:
                /// Number k of neighbors considered during training
                size_t k;
                /// Function to compute the metrics between two points
                Callable dist_function;
            public:
                KNNRegressor() = default;
                explicit KNNRegressor(const Data<T>& _samples, size_t _k=3):
                    PrimalRegressor<T> (mltk::make_data<T>(_samples)), k(_k) {}
                explicit KNNRegressor(size_t _k=3): k(_k) {}

                bool train() override{
                    return true;
                }

                std::string getFormulationString() override{
                    return "Regressor";
                }

                double evaluate(const Point <T> &p, bool raw_value = false) override{
                    auto points = this->samples->points();
                    std::vector<double> distances(this->samples->size());
                    std::vector<int> classes = this->samples->classes();
                    std::vector<size_t> idx(distances.size()), freq(classes.size());
                    auto p0 = std::make_shared<Point<T> >(p);
                    // fill the index vector
                    std::iota(idx.begin(), idx.end(), 0);

                    // compute the metrics from the sample to be evaluated to the samples vector
                    std::transform(points.begin(), points.end(), distances.begin(),
                                   [p0, this](const std::shared_ptr<Point<T> > q) {
                                       return this->dist_function(*p0, *q);
                                   });
                    // sort the index vector by the metrics from the sample to be evaluated
                    std::stable_sort(idx.begin(), idx.end(), [&distances](size_t i1, size_t i2) {
                        return distances[i1] < distances[i2];
                    });

                    double sum = 0.0;

                    // sum the values in the k nearest neighbors and return the average
                    for (size_t j = 0; j < this->k; j++) {
                        sum += points[idx[j]]->Y();
                    }
                    return sum / this->k;
                }
            };
        }
}
