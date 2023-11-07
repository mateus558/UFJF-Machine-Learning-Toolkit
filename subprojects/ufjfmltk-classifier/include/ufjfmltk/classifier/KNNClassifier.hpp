//
// Created by mateus558 on 20/03/2020.
//

#ifndef UFJF_MLTK_KNN_HPP
#define UFJF_MLTK_KNN_HPP

#include "PrimalClassifier.hpp"
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
                mltk::Point<mltk::Point<double>> distances;

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

                void setPrecomputedDistances(mltk::Point<mltk::Point<double>> _distances){
                    this->distances = _distances;
                    this->precomputed = true;
                }

                mltk::Point<mltk::Point<double>> precompute(mltk::Data<T> &data, const size_t threads = std::thread::hardware_concurrency());
            };
            

            template<typename T, typename Callable>
            mltk::Point<mltk::Point<double>> KNNClassifier<T, Callable>::precompute(mltk::Data<T> &data, const size_t threads){
                ThreadPool pool(threads);
                mltk::Point<mltk::Point<double>> distances(data.size());
                auto dist = Callable();

                for(size_t i = 0; i < data.size(); i++){
                    distances[i] = mltk::Point<double>(data.size());
                }

                std::cout << data.size() << std::endl;

                std::cout << "distances: " << distances.size() << std::endl;

                auto loop = [&distances, data, this](const int a, const int b) {
                    for(size_t idx = a; idx < b; idx++) {
                        for(size_t j = 0; j < idx; j++){
                            distances[data(idx).Id()-1][data(j).Id()-1] = this->dist_function(data(idx), data(j));
                        }

                        for(size_t j = idx+1; j < data.size(); j++){
                            distances[data(idx).Id()-1][data(j).Id()-1] = this->dist_function(data(idx), data(j));
                        }    
                    }
                };

                pool.parallelize_loop(0, data.size(), loop, threads); 
                pool.wait_for_tasks();


                this->precomputed = true;
                this->distances = distances;

                return distances;
            }

            template<typename T, typename Callable>
            double KNNClassifier<T, Callable>::evaluate(const Point<T> &p, bool raw_value) {
                assert(this->samples->dim() == p.size());
                auto points = this->samples->points();
                std::vector<double> distances(this->samples->size());
                std::vector<int> classes = this->samples->classes();
                std::vector<size_t> idx(distances.size());
                std::vector<PointPointer<T>> neigh;
                auto p0 = std::make_shared<Point<T> >(p);

                if(precomputed && this->distances.size() == 0){
                    precompute(*this->samples);
                }

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
                    }else{
                        std::nth_element(idx.begin(), idx.begin() + this->k, idx.end(), [&p, this, &points](size_t i1, size_t i2) {
                            return this->distances[p.Id()-1][points[i1]->Id()-1] < this->distances[p.Id()-1][points[i2]->Id()-1];
                        }); 
                    }
                }
                // find the most frequent class in the k nearest neighbors
                size_t max_index = 0, max_freq = 0, i=0;
                double s = 0.0001, max_prob = 0.0;
                std::for_each(classes.begin(), classes.end(), [&idx, &points, &i, &max_freq, &max_index, &max_prob, &s, &classes,&neigh, this](int c){
                    int freq = 0;
                    if(algorithm == "brute") {
                        freq = std::count_if(idx.begin(), idx.begin() + this->k, [&points, &c](size_t id) {
                            return points[id]->Y() == c;
                        });
                    }else{
                        freq = std::count_if(neigh.begin(), neigh.end(), [&c](auto point) {
                            return point->Y() == c;
                        });
                    }

                    if(freq > max_freq){
                        double prob = (freq+s)/(k+classes.size()*s);
                        max_index = i;
                        max_freq = freq;
                        max_prob = prob;
                    }
                    i++;
                });
                this->pred_prob = (1-max_prob > 1E-7) ? 1: max_prob;
                return classes[max_index]; 
            }

            template<typename T, typename Callable>
            bool KNNClassifier<T, Callable>::train() {
                return true;
            }
        }
}

#endif //UFJF_MLTK_KNN_HPP
