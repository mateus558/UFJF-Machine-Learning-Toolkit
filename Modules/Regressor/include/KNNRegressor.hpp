#ifndef UFJF_MLTK_KNNREGRESSOR_HPP
#define UFJF_MLTK_KNNREGRESSOR_HPP


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
        const size_t _dimp = p->X().size();
        size_t i;
        double dist = 0;

        for(i = 0; i < _dimp; i++){
            dist += (p->X()[i] - q->X()[i]) * (p->X()[i] - q->X()[i]);
        }
        return sqrt(dist);
    });

    bool train() override;

    std::string getFormulationString() override;

    double evaluate(Point<T> p, bool raw_value=false) override;
};

#endif