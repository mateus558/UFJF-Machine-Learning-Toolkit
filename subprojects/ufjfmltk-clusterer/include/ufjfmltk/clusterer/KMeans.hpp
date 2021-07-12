//
// Created by mateus558 on 26/03/2020.
//

#ifndef UFJF_MLTK_KMEANS_HPP
#define UFJF_MLTK_KMEANS_HPP
#pragma once

#include "ufjfmltk/clusterer/Clusterer.hpp"

namespace mltk{
        namespace clusterer {
            /**
             * \brief Wrapper for the implementation of the K-Means clustering algorithm.
             */
            template<typename T, typename Callable = metrics::dist::Euclidean<T> >
            class KMeans : public Clusterer<T> {
            private:
                /// Algorithm used for the initialization of the K-Means algorithm
                std::string initialization;

            public:
                KMeans() = default;
                KMeans(const Data<T>& samples, size_t k, std::string initialization = "random", size_t seed = 0,
                       int verbose = 0);

                bool train() override;

                double assign_clusters(const std::vector<mltk::PointPointer<T>>& points);

                void compute_centers();

                double evaluate(const Point<T> &p, bool raw_value = false) override;

                std::vector<mltk::Point<size_t>> clusters() override;

                std::string getFormulationString() override;

            };

            template<typename T, typename Callable>
            KMeans<T, Callable>::KMeans(const mltk::Data<T>& samples, size_t k, std::string initialization, size_t seed,
                                        int verbose)
                    : Clusterer<T>(mltk::make_data<T>(samples), k), initialization(std::move(initialization)){
                this->m_centers.assign(this->n_clusters, std::vector<T>(this->samples->dim(), 0.0));
                this->m_clusters.assign(this->n_clusters, std::vector<size_t>());
                this->verbose = verbose;
                this->seed = seed;
            }

            template<typename T, typename Callable>
            double KMeans<T, Callable>::assign_clusters(const std::vector<mltk::PointPointer<T>>& points) {
                double sum_d = 0;
                double cost = 0.0;

                std::for_each(points.begin(), points.end(), [&](const std::shared_ptr<Point<T> > q){
                    std::vector<double> distances(this->m_centers.size(), 0.0);
                    std::transform(this->m_centers.begin(), this->m_centers.end(), distances.begin(), [&](const Point<T>& center){
                        return this->dist_function(*q, center);
                    });
                    size_t cluster_id = std::min_element(distances.begin(), distances.end()) - distances.begin();
                    this->m_clusters[cluster_id].X().push_back(q->Id()-1);
                    cost += distances[cluster_id] * distances[cluster_id];
                });
                return cost;
            }

            template<typename T, typename Callable>
            void KMeans<T, Callable>::compute_centers(){
                int i = 0;
                std::for_each(this->m_clusters.begin(), this->m_clusters.end(), [&](const auto& cluster){
                    for(const size_t id: cluster){
                        this->m_centers[i] += *this->samples->point(id);
                    }
                    i++;
                });
                i = 0;
                std::for_each(this->m_centers.begin(), this->m_centers.end(), [&](auto& center){
                   center /= this->m_clusters[i].size();
                   i++;
                });
            }

            template<typename T, typename Callable>
            bool KMeans<T, Callable>::train() {
                double cost = 0.0, old_cost = 0.0;
                size_t dim = this->samples->dim(), size = this->samples->size();
                size_t time = this->start_time + this->max_time;
                auto points = this->samples->points();
                bool has_converged = true;
                mltk::random::init(this->seed);

                std::shuffle(points.begin(), points.end(), mltk::random::generator);

                if (initialization == "random") {
                    std::vector<size_t> centers_ids(this->n_clusters);

                    // generate the ids of the centers in the dataset using the random number generator function
                    std::generate(centers_ids.begin(), centers_ids.end(), [&size](){
                        return mltk::random::intInRange(0, size);
                    });
                    // get the values from the dataset for the centers
                    std::transform(centers_ids.begin(), centers_ids.end(), this->m_centers.begin(),
                                   [this](const size_t &center_id) {
                                       return (*this->samples)[center_id]->X();
                                   });
                } else if (initialization == "kmeanspp") {
                    // choose the first center randomly
                    this->m_centers[0] = points[mltk::random::intInRange(0, size)]->X();
                    // choose the next cluster in points with a probability directly proportional to the metrics from the
                    // last chosen cluster.
                    for (size_t i = 1; i < this->m_centers.size(); i++) {
                        double sum_d = 0.0;
                        std::vector<double> distances(points.size(), 0.0);
                        //compute the distances from the points to the last cluster
                        std::transform(points.begin(), points.end(), distances.begin(),
                                       [&i, &sum_d, this](const std::shared_ptr<Point<T> > q) {
                                           double d = this->dist_function(Point<T>(this->m_centers[i - 1]), *q);
                                           sum_d += d;
                                           return d;
                                       });
                        for (double &distance : distances) {
                            distance /= sum_d;
                        }
                        // use the distances as a probability distribution
                        std::discrete_distribution<size_t> _dist(distances.begin(), distances.end());
                        // generate the id for the next cluster
                        size_t center = mltk::random::intInRange(0, size);
                        this->m_centers[i] = points[center]->X();
                    }
                }

                if (this->verbose) {
                    std::cout << "------------------------------------------------------------------------------------\n";
                    std::cout << " steps     updates              cost_function              diff          milli(ms)\n";
                    std::cout << "------------------------------------------------------------------------------------\n";
                }
                this->timer.reset();
                do {
                    old_cost = cost;
                    has_converged = true;
                    // assign each point to the nearest cluster
                    cost = assign_clusters(points);
                    // update the centers of the clusters
                    compute_centers();
                    for(auto& cluster: this->m_clusters){
                        cluster.clear();
                    }
                    this->steps++;
                    double secs = this->timer.elapsed();
                    if (this->verbose) {
                        auto diff = fabs(cost - old_cost);
                        std::cout << " " << this->steps << "           " << this->ctot << "                   " << cost
                                  << "            " << diff << "           " << secs << "\n";
                    }
                    if (time - this->timer.elapsed() <= 0) break;
                    has_converged = fabs(cost - old_cost) <= this->EPS;
                } while (!has_converged);

                return true;
            }

            template<typename T, typename Callable>
            double KMeans<T, Callable>::evaluate(const Point<T> &p, bool raw_value) {
                std::vector<double> distances(this->m_centers.size(), 0.0);
                std::transform(this->m_centers.begin(), this->m_centers.end(), distances.begin(), [&](const Point<T>& center){
                    return this->dist_function(center, p);
                });
                size_t cluster_id = std::min_element(distances.begin(), distances.end()) - distances.begin();
                return cluster_id;
            }

            template<typename T, typename Callable>
            std::string KMeans<T, Callable>::getFormulationString() {
                return "Clusterer";
            }

            template<typename T, typename Callable>
            std::vector<mltk::Point<size_t>> KMeans<T, Callable>::clusters() {
                assign_clusters(this->samples->points());
                return this->m_clusters;
            }

        }
}

#endif //UFJF_MLTK_KMEANS_HPP
