#ifndef UFJF_MLTK_KNNREGRESSOR_HPP
#define UFJF_MLTK_KNNREGRESSOR_HPP


#include "PrimalRegressor.hpp"
#include "DistanceMetric.hpp"
#include <assert.h>
#include <functional>

template <typename T, typename Callable = EuclideanDistance< T > >
class KNNRegressor: public PrimalRegressor< T > {
private:
    size_t k;
    Callable dist_function;
public:
    KNNRegressor(std::shared_ptr<Data<T> > _samples, size_t _k, Callable dist_func = Callable())
    : PrimalRegressor< T >(_samples), k(_k), dist_function(dist_func) {}

    bool train() override;

    std::string getFormulationString() override;

    double evaluate(Point<T> p, bool raw_value=false) override;
};

#endif