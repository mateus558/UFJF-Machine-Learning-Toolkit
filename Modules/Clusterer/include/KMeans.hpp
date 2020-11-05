//
// Created by mateus558 on 26/03/2020.
//

#ifndef UFJF_MLTK_KMEANS_HPP
#define UFJF_MLTK_KMEANS_HPP

#include "Clusterer.hpp"

template<typename T>
class KMeans: public Clusterer< T > {
private:
    using Function = typename Clusterer< T >::Function;

    std::string initialization;

public:
    KMeans(std::shared_ptr<Data<T> > _samples, size_t k, const std::string& initialization = "random", Function _dist_function  = [] (const std::shared_ptr<Point< double > > p, const std::shared_ptr<Point< T > > q){
        const size_t _dimp = p->X().size();
        size_t i;
        double dist = 0;

        for(i = 0; i < _dimp; i++){
            dist += (p->X()[i] - q->X()[i]) * (p->X()[i] - q->X()[i]);
        }
        return sqrt(dist);
    });

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;

    std::string getFormulationString() override;

};


#endif //UFJF_MLTK_KMEANS_HPP
