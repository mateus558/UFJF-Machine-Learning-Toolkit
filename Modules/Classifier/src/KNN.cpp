//
// Created by mateus558 on 20/03/2020.
//

#include "KNN.hpp"
#include <memory>

namespace mltk{
    
    template <typename T, typename Callable >
    double KNNClassifier<T, Callable>::evaluate(const Point< T >  &p, bool raw_value) {
        size_t max_index = 0, max_freq = 0, i;
        auto points = this->samples->getPoints();
        std::vector<double> distances(this->samples->getSize());
        std::vector<int> classes = this->samples->getClasses();
        std::vector<size_t> idx(distances.size()), freq(classes.size());
        auto p0 = std::make_shared<Point< T > >(p);

        // fill the index vector
        std::iota(idx.begin(), idx.end(), 0);
        // compute the distance from the sample to be evaluated to the samples vector
        std::transform(points.begin(), points.end(), distances.begin(), [p0, this](const std::shared_ptr<Point< T > > q){
            return this->dist_function(*p0, *q);
        });
        // sort the index vector by the distance from the sample to be evaluated
        std::stable_sort(idx.begin(), idx.end(), [&distances](size_t i1, size_t i2){
            return distances[i1] < distances[i2];
        });

        // find the most frequent class in the k nearest neighbors
        for(size_t j = 0; j < this->k; j++){
            for (i = 0; i < classes.size(); i++) {
                if (classes[i] == points[idx[j]]->Y()) {
                    freq[i]++;
                    if (freq[i] > max_freq) {
                        max_freq = freq[i];
                        max_index = i;
                    }
                }
            }
        }
        return classes[max_index];
    }

    template <typename T, typename Callable >
    bool KNNClassifier<T, Callable>::train() {
        return true;
    }


    template class KNNClassifier<int>;
    template class KNNClassifier<double>;
    template class KNNClassifier<float>;
    template class KNNClassifier<int8_t>;
    template class KNNClassifier<char>;
    template class KNNClassifier<long long int>;
    template class KNNClassifier<short int>;
    template class KNNClassifier<long double>;
    template class KNNClassifier<unsigned char>;
    template class KNNClassifier<unsigned int>;
    template class KNNClassifier<unsigned short int>;
}