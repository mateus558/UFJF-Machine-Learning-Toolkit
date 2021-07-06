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

                double evaluate(const Point<T> &p, bool raw_value = false) override;

                std::vector<std::vector<size_t>> clusters() override;

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
                    cost = 0.0;
                    has_converged = true;

                    // assign each point to the nearest cluster
                    for (size_t i = 0; i < size; i++) {
                        std::vector<double> distances(this->n_clusters, 0.0);
                        double min_value = std::numeric_limits<double>::max();
                        size_t min_cluster = 0;

                        for (size_t c = 0; c < this->n_clusters; c++) {
                            auto point = (*this->samples)[i]->X();
                            auto center = this->m_centers[c];
                            std::vector<T> diff(dim);
                            // compute the difference between the pointer in a cluster to it's center
                            for (size_t j = 0; j < dim; j++) {
                                diff[j] = point[j] - center[j];
                            }
                            distances[c] = std::sqrt(
                                    (double) std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0));
                            if (distances[c] < min_value) {
                                min_value = distances[c];
                                min_cluster = c;
                            }
                        }
                        cost += distances[min_cluster] * distances[min_cluster];
                        this->m_clusters[min_cluster].push_back(i);
                    }

                    // update the centers of the clusters
                    for (size_t c = 0; c < this->n_clusters; c++) {
                        size_t cluster_size = this->m_clusters[c].size();
                        this->m_centers[c].assign(dim, T());
                        for (size_t e = 0; e < cluster_size; e++) {
                            for (size_t j = 0; j < dim; j++) {
                                this->m_centers[c][j] += points[this->m_clusters[c][e]]->X()[j];
                            }
                        }
                        for (size_t j = 0; j < dim; j++) {
                            this->m_centers[c][j] /= cluster_size;
                        }
                    }
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
                std::vector<double> dists(this->m_centers.size());
                auto compute_distances = [p, this](std::vector<T> center){
                    return this->dist_function(p, mltk::Point<T>(center));
                };
                std::transform(this->m_centers.begin(), this->m_centers.end(), dists.begin(), compute_distances);
                auto closest_center = std::min_element(dists.begin(), dists.end()) - dists.begin()+1;
                return closest_center;
            }

            template<typename T, typename Callable>
            std::string KMeans<T, Callable>::getFormulationString() {
                return "Clusterer";
            }

            template<typename T, typename Callable>
            std::vector<std::vector<size_t>> KMeans<T, Callable>::clusters() {
                for(size_t i = 0; i < this->samples->size(); i++){
                    auto point = (*this->samples)[i];
                    std::vector<double> dists(this->m_centers.size());
                    auto compute_distances = [point, this](std::vector<T> center){
                        return this->dist_function(*point, mltk::Point<T>(center));
                    };
                    std::transform(this->m_centers.begin(), this->m_centers.end(), dists.begin(), compute_distances);
                    auto closest_center = std::min_element(dists.begin(), dists.end()) - dists.begin();
                    this->m_clusters[closest_center].push_back(i);
                }
                return this->m_clusters;
            }

        }
}

#endif //UFJF_MLTK_KMEANS_HPP
