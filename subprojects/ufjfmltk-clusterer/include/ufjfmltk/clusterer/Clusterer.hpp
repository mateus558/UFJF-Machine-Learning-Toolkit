//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_CLUSTERER_HPP
#define UFJF_MLTK_CLUSTERER_HPP
#pragma once

#include "ufjfmltk/core/DistanceMetric.hpp"
#include "ufjfmltk/core/Learner.hpp"

namespace mltk{
        /**
         * \brief Namespace for clustering methods.
         */
        namespace clusterer {
            template<typename T, typename Callable = metrics::dist::Euclidean <T> >
            class Clusterer : public Learner<T> {
            protected:
                /// Function used to compute the metrics between two points
                Callable dist_function;
                /// Number of clusters for the cluster method
                size_t n_clusters{};
                /// Vector with the centers of the clusters
                std::vector<mltk::Point<T> > m_centers;
                /// Clusters of points
                std::vector<mltk::Point<size_t> > m_clusters;

            public:
                Clusterer() = default;

                explicit Clusterer(DataPointer <T> samples = nullptr, size_t clusters = 0)
                        : Learner<T>(samples), n_clusters(clusters) {}

                virtual std::vector<mltk::Point<size_t> > clusters() { return m_clusters; }
                std::vector<mltk::Point<T> > centers() { return m_centers; }
            };
        }
}

#endif //UFJF_MLTK_CLUSTERER_HPP
