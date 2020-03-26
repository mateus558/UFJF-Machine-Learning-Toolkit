//
// Created by mateus558 on 26/03/2020.
//

#include <random>
#include "../includes/KMeans.hpp"

template<typename T>
bool KMeans<T>::train() {
    double cost = 0.0, ant_cost = 0.0;
    size_t dim = this->samples->getDim(), size = this->samples->getSize();
    auto points = this->samples->getPoints();
    bool has_converged = false;
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_int_distribution<size_t> dist {0, this->samples->getSize()};

    if(initialization == "random"){
        std::vector<size_t> centers_ids(this->n_clusters);

        // create a random number generator function
        auto gen = [&dist, &mersenne_engine](){
            return dist(mersenne_engine);
        };
        // generate the ids of the centers in the dataset using the random number generator function
        std::generate(centers_ids.begin(), centers_ids.end(), gen);
        // get the values from the dataset for the centers
        std::transform(centers_ids.begin(), centers_ids.end(), this->centers.begin(), [this](const size_t &center_id){
            return (*this->samples)[center_id]->x;
        });
    }else if(initialization == "kmeanspp"){
        // choose the first center randomly
        this->centers[0] = (*this->samples)[dist(mersenne_engine)]->x;
        // choose the next cluster in points with a probability directly proportional to the distance from the
        // last chosen cluster.
        for(size_t i = 1; i < this->centers.size(); i++){
            std::vector<double> distances(points.size(), 0.0);
            //compute the distances from the points to the last cluster
            std::transform(points.begin(), points.end(), distances.begin(), [&i, this](const std::shared_ptr<Point< T > > q){
                return this->dist_function(std::make_shared<Point<T> >(this->centers[i-1]), q);
            });
            // use the distances as a probability distribution
            std::discrete_distribution<size_t> _dist(distances.begin(), distances.end());
            // generate the id for the next cluster
            size_t center = _dist(mersenne_engine);
            this->centers[i] = points[center]->x;
        }
    }

    do{
        for(size_t i = 0; i < this->n_clusters; i++){
            this->clusters[i].erase(this->clusters[i].begin(), this->clusters[i].end());
        }
        // assign each point to the nearest cluster
        for(size_t i = 0; i < size; i++){
            std::vector<double> distances(this->n_clusters, 0.0);
            double min_value = std::numeric_limits<double>::max();
            size_t min_cluster = 0;

            for(size_t c = 0; c < this->n_clusters; c++){
                distances[c] = this->dist_function(std::make_shared<Point<T> >(this->centers[c]), points[i]);
                if(distances[c] < min_value) {
                    min_value = distances[c];
                    min_cluster = c;
                }
            }
            this->clusters[min_cluster].push_back(i);
        }
        // update the centers of the clusters
        for(size_t c = 0; c < this->n_clusters; c++){
            size_t cluster_size = this->clusters[c].size();
            this->centers[c] = std::vector<T>(dim, T());
            for(size_t e = 0; e < cluster_size; e++){
                for(size_t j = 0; j < dim; j++) {
                    this->centers[c][j] += points[e]->x[j];
                }
            }

            for(size_t i = 0; i < dim; i++){
                this->centers[c][i] /= cluster_size;
            }
        }

        std::vector<double> costs(this->n_clusters);

        ant_cost = cost;
        for(size_t i = 0; i < this->n_clusters; i++){
            for(auto c: this->clusters[i]){
                auto point = (*this->samples)[c]->x;
                auto center = this->centers[i];
                std::vector< T > diff(dim);
                // compute the difference between the pointer in a cluster to it's center
                for(size_t j = 0; j < dim; j++){
                    diff[j] = point[j] - center[j];
                }
                // compute the norm of the point and finally accumulate to the cost
                double norm = std::sqrt((double)std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0));
                costs[i] += norm * norm;
            }
        }
        // the total cost is the sum of the costs of all clusters
        cost =  std::accumulate(costs.begin(), costs.end(), 0.0);
        has_converged = std::fabs(cost - ant_cost) <= this->EPS;
    }while(!has_converged);

    return true;
}

template<typename T>
double KMeans<T>::evaluate(Point<T> p) {
    std::vector<double> distances(this->n_clusters, 0.0);
    size_t min_value = std::numeric_limits<size_t>::max();
    size_t min_cluster = 0;

    for(size_t c = 0; c < this->n_clusters; c++){
        distances[c] = this->dist_function(std::make_shared<Point<T> >(this->centers[c]), std::make_shared<Point<T> >(p));
        if(distances[c] < min_value){
            min_value = distances[c];
            min_cluster = c;
        }
    }
    return min_cluster+1;
}

template<typename T>
std::string KMeans<T>::getFormulationString() {
    return "Clusterer";
}

template<typename T>
KMeans<T>::KMeans(std::shared_ptr<Data<T>> _samples, size_t k, const std::string& initialization, Function _dist_function){
    this->samples = _samples;
    this->n_clusters = k;
    this->initialization = initialization;
    this->dist_function = _dist_function;

    this->centers.assign(this->n_clusters, std::vector<T>(this->samples->getDim(), 0.0));
    this->clusters.assign(this->n_clusters, std::vector<size_t>());
}

template class KMeans<int>;
template class KMeans<double>;
template class KMeans<float>;
template class KMeans<int8_t>;
template class KMeans<char>;
template class KMeans<long long int>;
template class KMeans<short int>;
template class KMeans<unsigned char>;
template class KMeans<unsigned int>;
template class KMeans<unsigned short int>;