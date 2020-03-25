//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "Clusterer.hpp"
#include <assert.h>

template <typename T>
class kNN: public Clusterer< T > {
public:
    kNN(std::shared_ptr<Data<T> > _samples, size_t k, std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >)> _dist_function  = [] (const std::shared_ptr<Point< T > > p, const std::shared_ptr<Point< T > > q){
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


#endif //UFJF_MLTK_KNN_HPP
