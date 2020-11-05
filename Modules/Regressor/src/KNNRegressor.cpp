#include "KNNRegressor.hpp"
#include <memory>

template<typename T>
KNNRegressor<T>::KNNRegressor(std::shared_ptr<Data<T>> _samples, size_t _k, KNNRegressor::function _dist_function) {
    this->samples = _samples;
    this->k = _k;
    this->dist_function = _dist_function;
}

template<typename T>
std::string KNNRegressor<T>::getFormulationString() {
    return "Regressor";
}

template<typename T>
double KNNRegressor<T>::evaluate(Point<T> p, bool raw_value) {
    auto points = this->samples->getPoints();
    std::vector<double> distances(this->samples->getSize());
    std::vector<int> classes = this->samples->getClasses();
    std::vector<size_t> idx(distances.size()), freq(classes.size());
    auto p0 = std::make_shared<Point< T > >(p);
    // fill the index vector
    std::iota(idx.begin(), idx.end(), 0);

    // compute the distance from the sample to be evaluated to the samples vector
    std::transform(points.begin(), points.end(), distances.begin(), [p0, this](const std::shared_ptr<Point< T > > q){
        return this->dist_function(p0, q);
    });
    // sort the index vector by the distance from the sample to be evaluated
    std::stable_sort(idx.begin(), idx.end(), [&distances](size_t i1, size_t i2){
        return distances[i1] < distances[i2];
    });

    double sum = 0.0;

    // sum the values in the k nearest neighbors and return the average
    for(size_t j = 0; j < this->k; j++){
        sum += points[idx[j]]->Y();
    }
    return sum/this->k;
}

template<typename T>
bool KNNRegressor<T>::train() {
    return false;
}

template class KNNRegressor<int>;
template class KNNRegressor<double>;
template class KNNRegressor<float>;
template class KNNRegressor<int8_t>;
template class KNNRegressor<char>;
template class KNNRegressor<long long int>;
template class KNNRegressor<short int>;
template class KNNRegressor<long double>;
template class KNNRegressor<unsigned char>;
template class KNNRegressor<unsigned int>;
template class KNNRegressor<unsigned short int>;
