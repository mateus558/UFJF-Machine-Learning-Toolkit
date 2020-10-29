//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "PrimalClassifier.hpp"
#include <assert.h>
#include <functional>

template <typename T>
class KNN: public PrimalClassifier< T > {
private:
    typedef std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >)> function;

    size_t k;
    function dist_function;

public:
    KNN(std::shared_ptr<Data<T> > _samples, size_t _k, function _dist_function  = [] (const std::shared_ptr<Point< T > > p, const std::shared_ptr<Point< T > > q){
        const size_t _dimp = p->x.size();
        size_t i;
        double dist = 0;

        for(i = 0; i < _dimp; i++){
            dist += (p->x[i] - q->x[i]) * (p->x[i] - q->x[i]);
        }
        return sqrt(dist);
    });

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;
};
/*

*/
#endif //UFJF_MLTK_KNN_HPP
