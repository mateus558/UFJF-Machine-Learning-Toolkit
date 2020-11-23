//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_CLUSTERER_HPP
#define UFJF_MLTK_CLUSTERER_HPP
#pragma once

#include "DistanceMetric.hpp"
#include "../../Core/include/Learner.hpp"

namespace mltk{
        namespace clusterer {
            template<typename T, typename Callable = distance::Euclidean <T> >
            class Clusterer : public Learner<T> {
            protected:
                /// Function used to compute the distance between two points
                Callable dist_function;
                /// Number of clusters for the cluster method
                size_t n_clusters;
                /// Vector with the centers of the clusters
                std::vector<std::vector<T> > centers;
                /// Clusters of points
                std::vector<std::vector<size_t> > clusters;

            public:
                Clusterer() {}

                Clusterer(DataPointer <T> samples = nullptr, size_t clusters = 0)
                        : Learner<T>(samples), n_clusters(clusters) {}
            };
        }
}

#endif //UFJF_MLTK_CLUSTERER_HPP
