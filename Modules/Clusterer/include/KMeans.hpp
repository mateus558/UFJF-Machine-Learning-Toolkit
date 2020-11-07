//
// Created by mateus558 on 26/03/2020.
//

#ifndef UFJF_MLTK_KMEANS_HPP
#define UFJF_MLTK_KMEANS_HPP

#include "Clusterer.hpp"

namespace mltk{
    /**
     * \brief Wrapper for the implementation of the K-Means clustering algorithm.
     */
    template<typename T, typename Callable = mltk::EuclideanDistance< T > >
    class KMeans: public Clusterer< T > {
    private:
        /// Algorithm used for the initialization of the K-Means algorithm
        std::string initialization;

    public:
        KMeans(std::shared_ptr<Data<T> > _samples, size_t k, const std::string& initialization = "random");

        bool train() override;

        double evaluate(Point<T> p, bool raw_value=false) override;

        std::string getFormulationString() override;

    };
}

#endif //UFJF_MLTK_KMEANS_HPP
