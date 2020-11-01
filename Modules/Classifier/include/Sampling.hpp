#ifndef OVERSAMPLING_HPP_INCLUDED
#define OVERSAMPLING_HPP_INCLUDED

#include "Data.hpp"
#include <random>

template < typename T >
class OverSampling{
public:
    OverSampling(){}
    void setSeed(size_t seed = 0){ this->seed = seed; }
    virtual DataPointer< T > operator()(DataPointer< T > data) = 0;
};

template < typename T >
class SMOTE: public OverSampling< T > {
private:
    size_t seed = 0;
    size_t k = 1;
    double r = 0.1;

public:
    SMOTE(size_t k = 1, double r = 0.1, size_t seed = 0): k(k), r(r), seed(seed) {}

    DataPointer< T > operator()(DataPointer< T > data) override {
        std::random_device rd;
        seed = (seed > 0)?seed:rd();
	    std::mt19937 generator(seed);
	    std::uniform_real_distribution<double> distribution(0.0,1.0); 
        // number of generated artificial points
        size_t n_apoints = r * data->getSize(); 
        // find the minority class
        auto classes = data->getClasses();
        auto class_distribution = data->getClassesDistribution();
        int min_class = classes[std::min_element(class_distribution.begin(), class_distribution.end()) - class_distribution.begin()];
        // copy all the points from the minority class to the Z set
        std::vector<int> class_copy = {min_class};
        std::vector<SamplePointer< T > > artificial_data;
        DataPointer< T > Z = std::make_shared<Data< T > >();
        Z->classesCopy(*data, class_copy);
        
        // iterate through all the elements from the Z set
        for(auto z = Z->begin(); z != Z->end(); ++z){
            std::vector<std::pair<size_t, double> > distance(data->getSize());
            std::vector<SamplePointer< T > > k_neighbors(k);
            size_t id = 0;
            auto _z = *z;

            // compute the euclidean distance from a point to the rest 
            std::transform(Z->begin(), Z->end(), distance.begin(), [&_z, &id](auto p){
                const size_t _dimp = p->x.size();
                size_t i;
                double dist = 0;

                for(i = 0; i < _dimp; i++){
                    dist += ((*_z).x[i] - p->x[i]) * ((*_z).x[i] - p->x[i]);
                }
                id++;
                return std::make_pair(id, sqrt(dist));
            });

            // sort the distances in decreasing order
            std::sort(distance.begin(), distance.end(), [](auto &d1, auto &d2){
                return d1.second > d2.second;
            });

            // get the k neighbors
            for(size_t i = 0; i < k; i++){
                k_neighbors[i] = (*data)[distance[i].first];
            }

            // create the artificial points and insert them to the dataset
            for(auto p = k_neighbors.begin(); p != k_neighbors.end(); p++){
                auto _k = (*p);
                Point< T > s(_z->x.size(), 0.0, 0);
                double alpha = distribution(generator);
                
                for(size_t d = 0; d < _z->x.size(); d++){
                    s.x[d] = _z->x[d] + alpha * (_z->x[d] - _k->x[d]);
                }

                s.y = min_class;
                artificial_data.push_back(std::make_shared<Point< T > >(s));
            }
        }

        std::shuffle(artificial_data.begin(), artificial_data.end(), std::default_random_engine(seed));
        for(size_t i = 0; i < n_apoints; i++){
            data->insertPoint(artificial_data[i]);
        }

        return data;
    }
}; 

template < typename T >
class BorderlineSMOTEOne: public OverSampling< T > {
private:
    size_t seed = 0;
    size_t k = 1;
    size_t m = 1;
    double r = 0.1;
    
public:
    BorderlineSMOTEOne(size_t k = 1, double r = 0.1, size_t m = 1, size_t seed = 0): k(k), r(r), m(m), seed(seed) {}

    DataPointer< T > operator()(DataPointer< T > data) override {
        // Find the majority class
        auto classes = data->getClasses();
        auto class_distribution = data->getClassesDistribution();
        int maj_class = classes[std::max_element(class_distribution.begin(), class_distribution.end()) - class_distribution.begin()];
        // Initialize the danger subset
        DataPointer< T > danger_subset = std::make_shared<Data< T > >();
        std::set<size_t> danger_ids;

        // iterate through all the elements from the Z set
        for(auto z = data->begin(); z != data->end(); ++z){
            std::vector<std::pair<size_t, double> > distance(data->getSize());
            std::vector<SamplePointer< T > > M(m);
            size_t id = 0;
            auto _z = *z;

            // compute the euclidean distance from a point to the rest 
            std::transform(data->begin(), data->end(), distance.begin(), [&_z, &id](auto p){
                const size_t _dimp = p->x.size();
                size_t i;
                double dist = 0;

                for(i = 0; i < _dimp; i++){
                    dist += ((*_z).x[i] - p->x[i]) * ((*_z).x[i] - p->x[i]);
                }
                id++;
                return std::make_pair(id, sqrt(dist));
            });

            // sort the distances in decreasing order
            std::sort(distance.begin(), distance.end(), [](auto &d1, auto &d2){
                return d1.second > d2.second;
            });

            // get the m neighbors
            for(size_t i = 0; i < m; i++){
                M[i] = (*data)[distance[i].first];
            }
            
            // set m' as the number of points on the majority class set on m neighbors
            size_t m_ = std::count_if(M.begin(), M.end(), [&maj_class](auto &p){
                return p->y == maj_class;
            });
            
            // insert the point to the danger subset if m/2 <= m' < m
            //std::cout << m_ << " " << m/2 << " " << m << std::endl;
            if(m_ >= (m/2) && m_ < m){
                danger_ids.insert(_z->id);
                danger_subset->insertPoint(_z);
            }
        }

        // apply SMOTE algorithm to the danger subset
        if(danger_subset->getSize() > 0){
            std::shared_ptr<SMOTE< T > > smote = std::make_shared< SMOTE< T > >(k, r, seed);
            (*smote)(danger_subset);
        }

        for(auto p = danger_subset->begin(); p != danger_subset->end(); p++){
            auto _p = (*p);

            if(danger_ids.find(_p->id) != danger_ids.end()){
                data->insertPoint(_p);
            }
        }

        return data;
    }
};

#endif //OVERSAMPLING_HPP_INCLUDED