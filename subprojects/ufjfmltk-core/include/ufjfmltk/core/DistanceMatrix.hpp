#pragma once

#include <thread>

#include "ThreadPool.hpp"
#include "DistanceMetric.hpp"

namespace mltk::metrics::dist {
    template<typename DistanceFunc = metrics::dist::Euclidean<double> >
    class DistanceMatrix {
    private:
        mltk::Point<double>::Matrix distances;
        bool isDiagonal{false};
        size_t threads{ std::thread::hardware_concurrency() };
        DistanceFunc dist_function{};

        void compute(const mltk::Data<double> &data) {
            mltk::ThreadPool pool(threads);
            
            auto loop = [data, this](const int a, const int b) {
                for(size_t idx = a; idx < b; idx++) {
                    this->distances[idx] = mltk::Point<double>((isDiagonal) ? idx+1 : data.size());

                    for(size_t j = 0; j < idx; j++){
                        this->distances[data(idx).Id()-1][data(j).Id()-1] = this->dist_function(data(idx), data(j));
                    }
                    
                    if(isDiagonal) continue;

                    for(size_t j = idx+1; j < data.size(); j++){
                        this->distances[data(idx).Id()-1][data(j).Id()-1] = this->dist_function(data(idx), data(j));
                    }    
                }
            };

            pool.parallelize_loop(0, data.size(), loop, threads); 
            pool.wait_for_tasks();
        }

    public:
        DistanceMatrix() = default;
        
        explicit DistanceMatrix(mltk::Data<double> &data, const bool isDiagonal = false, const size_t threads = std::thread::hardware_concurrency()) {
            this->threads = threads;
            this->isDiagonal = isDiagonal;
            this->distances = mltk::Point<double>::Matrix(data.size());
            
            this->compute(data);
        }

        bool isDiagonalMatrix() const {return this->isDiagonal;}

        size_t size() const {return this->distances.size();}

        mltk::Point<double> operator[](size_t i) const {return this->distances[i];}

        mltk::Point<double> & operator[](size_t i) {return this->distances[i];}
    };
}