#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP


#include "PrimalRegressor.hpp"
#include <assert.h>
#include <functional>

template <typename T>
class KNNRegressor: public PrimalRegressor< T > {
private:
    using function = std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >)>;

    size_t k;
    function dist_function;
public:
    KNNRegressor(std::shared_ptr<Data<T> > _samples, size_t _k, function _dist_function  = [] (const std::shared_ptr<Point< T > > p, const std::shared_ptr<Point< T > > q){
        const size_t _dimp = p->x.size();
        size_t i;
        double dist = 0;

        for(i = 0; i < _dimp; i++){
            dist += (p->x[i] - q->x[i]) * (p->x[i] - q->x[i]);
        }
        return sqrt(dist);
    });

    bool train() override;

    std::string getFormulationString() override;

    double evaluate(Point<T> p) override;
};

#endif