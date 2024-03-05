//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "PrimalClassifier.hpp"
#include "ufjfmltk/core/DistanceMatrix.hpp"
#include "ufjfmltk/core/DistanceMetric.hpp"
#include "ufjfmltk/core/ThreadPool.hpp"
#include <assert.h>


namespace mltk{
        namespace classifier {
            /**
             * \brief Wrapper for the implementation of the K-Nearest Neighbors classifier algorithm.
             */
            template<typename T = double, typename Callable = metrics::dist::Euclidean<T> >
            class KNNClassifier : public PrimalClassifier<T> {
                private:
                    /// Number k of neighbors to be considered
                    size_t k = 3;
                    /// Function to compute the metrics between two points
                    Callable dist_function;
                    /// Enables the use of precomputed distances
                    bool precomputed = false;
                    std::string algorithm = "brute";
                    metrics::dist::BaseMatrix distances;

                public:

                    KNNClassifier() = default;
                    explicit KNNClassifier(size_t _k, std::string _algorithm = "brute")
                            : k(_k), algorithm(_algorithm) {}

                    KNNClassifier(Data<T> &_samples, size_t _k, std::string _algorithm = "brute")
                            : k(_k), algorithm(_algorithm) {
                        this->samples = mltk::make_data<T>(_samples);
                    }

                    bool train() override;

                    double evaluate(const Point<T> &p, bool raw_value = false) override;

                    Callable& metric(){ return dist_function; }

                    void setPrecomputedDistances(metrics::dist::BaseMatrix _distances){
                        this->distances = _distances;
                        this->precomputed = true;
                    }

                    metrics::dist::DistanceMatrix<Callable> precomputeDistances(mltk::Data<T> &data, bool diagonal = false, const size_t threads = std::thread::hardware_concurrency()){
                        this->precomputed = true;

                        return metrics::dist::DistanceMatrix<Callable>(data, diagonal, threads);
                    }
            };
            

            template<typename T, typename Callable>
            double KNNClassifier<T, Callable>::evaluate(const Point<T> &p, bool raw_value) {
                assert(this->samples->dim() == p.size());
                auto points = this->samples->points();
                mltk::Point<double> distances(this->samples->size());
                std::vector<int> classes = this->samples->classes();
                std::vector<size_t> idx(distances.size());
                std::vector<PointPointer<T>> neigh;
                auto p0 = std::make_shared<Point<T> >(p);
               
                if(algorithm == "brute"){
                    // fill the index vector
                    std::iota(idx.begin(), idx.end(), 0);
                    if(!precomputed) {   
                        // compute the metrics from the sample to be evaluated to the samples vector
                        std::transform(points.begin(), points.end(), distances.begin(),
                                    [&p0, this](const std::shared_ptr<Point<T> > q) {
                                        return this->dist_function(*p0, *q);
                                    });


                        // sort the index vector by the metrics from the sample to be evaluated
                        std::nth_element(idx.begin(), idx.begin() + this->k, idx.end(), [&distances](size_t i1, size_t i2) {
                            return distances[i1] < distances[i2];
                        });
                    }else if(!this->distances.isDiagonalMatrix()){
                        size_t idp = p.Id()-1;
                        std::nth_element(idx.begin(), idx.begin() + this->k, idx.end(), [&idp, &distances, &points, this](size_t i1, size_t i2) {
                            size_t id1 = points[i1]->Id()-1;
                            size_t id2 = points[i2]->Id()-1;

                            return this->distances[idp][id1] < this->distances[idp][id2];
                        }); 
                    } else { 
                        std::nth_element(idx.begin(), idx.begin() + this->k, idx.end(), [&p, this, &points](size_t i1, size_t i2) {
                            size_t id1 = points[i1]->Id()-1;
                            size_t id2 = points[i2]->Id()-1;
                            size_t idp = p.Id()-1;
                            
                            if(idp == id1 || idp == id2) return false;

                            size_t idp1 = (idp > id1) ? idp : id1;
                            size_t id1p = (idp > id1) ? id1 : idp;
                            
                            size_t idp2 = (idp > id2) ? idp : id2;
                            size_t id2p = (idp > id2) ? id2 : idp;

                            return this->distances[idp1][id1p] < this->distances[idp2][id2p];
                        });
                    }
                }
               // Define frequency counting logic in a lambda function.
                auto calculateFrequency = [&idx, &points, &neigh, this](int c) {
                    if (algorithm == "brute") {
                        return std::count_if(idx.begin(), idx.begin() + this->k, [&points, &c](size_t id) { 
                            return points[id]->Y() == c; 
                        });
                    } else {
                        return std::count_if(neigh.begin(), neigh.end(), [&c](auto point) { 
                            return point->Y() == c; 
                        });
                    }
                };

                // Use std::pair to store max frequency and its related index.
                std::pair<int, size_t> maxDetails{0, 0}; // frequency, index

                double s = 0.0001;
                double max_prob = 0.0;

                // Iterate over classes using a conventional for loop for better readability.
                for(size_t i = 0; i < classes.size(); ++i) {
                    int freq = calculateFrequency(classes[i]);

                    if(freq > maxDetails.first) {
                        double prob = (freq+s)/(k+classes.size()*s);
                        
                        maxDetails.first = freq;
                        maxDetails.second = i; 

                        max_prob = prob;
                    }
                }

                this->pred_prob = (1-max_prob > 1E-7) ? 1: max_prob;

                return classes[maxDetails.second];
            }

            template<typename T, typename Callable>
            bool KNNClassifier<T, Callable>::train() {
                return true;
            }
        }
    }
#endif //UFJF_MLTK_KNN_HPP
