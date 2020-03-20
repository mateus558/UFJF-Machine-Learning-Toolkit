//
// Created by mateus558 on 20/03/2020.
//

#include "../includes/kNN.hpp"
#include <memory>

template <typename T >
kNN< T >::kNN(std::shared_ptr<Data<T>> _samples, size_t k, std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >)> _dist_function){
    this->samples = _samples;
    this->n_clusters = k;
    this->dist_function = _dist_function;
}

template <typename T >
double kNN< T >::evaluate(Point<T> p) {
    size_t max_index = 0, i, max_freq = 0;
    auto points = this->samples->getPoints();
    std::vector<double> distances(this->samples->getSize());
    std::vector<int> classes = this->samples->getClasses();
    std::vector<size_t> idx(distances.size()), freq(classes.size());
    auto p0 = std::make_shared<Point< T > >(p);

    // fill the index vector
    std::iota(idx.begin(), idx.end(), 0);

    // compute the distance from the sample to be evaluated to the samples vector
    std::transform(points.begin(), points.end(), distances.begin(), [p0, this](std::shared_ptr<Point< T > > q){
        return this->dist_function(p0, q);
    });

    // sort the index vector by the distance from the sample to be evaluated
    std::stable_sort(idx.begin(), idx.end(), [&distances](size_t i1, size_t i2){
        return distances[i1] < distances[i2];
    });

    bool is_classification = this->samples->getType() == "Classification";
    double sum = 0.0;

    // find the most frequent class in the k nearest neighbors
    for(size_t j = 0; j < this->n_clusters; j++){
        size_t id = idx[j];
        if(is_classification){
            for (i = 0; i < classes.size(); i++) {
                if (classes[i] == points[id]->y) {
                    freq[i]++;
                    if (freq[i] > max_freq) {
                        max_freq = freq[i];
                        max_index = i;
                    }
                }
            }
        }else{
            sum += points[id]->y;
        }
    }
    return (is_classification)?classes[max_index]:(sum/this->n_clusters);
}

template<typename T>
bool kNN<T>::train() {
    return false;
}

template<typename T>
std::string kNN<T>::getFormulationString() {
    return "Clusterer";
}

template class kNN<int>;
template class kNN<double>;
template class kNN<float>;
template class kNN<int8_t>;
template class kNN<char>;
template class kNN<long long int>;
template class kNN<short int>;
template class kNN<long double>;
template class kNN<unsigned char>;
template class kNN<unsigned int>;
template class kNN<unsigned short int>;