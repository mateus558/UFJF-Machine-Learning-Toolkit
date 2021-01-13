//
// Created by mateuscmarim on 07/01/2021.
//

#include "../Modules/Core/Core.hpp"

using namespace mltk;

int main(int argc, char* argv[]){
    Data<double> data("iris.data");
    metrics::BallTree<double> ballTree(data);

    std::clog << "Number of nodes: " << metrics::BallTree<double>::nodes_count << std::endl;
    std::clog << "Dataset: breast\nSize: " << data.getSize() << "\nDimensions: " << data.getDim() << std::endl;
    std::clog << std::endl;
    Point<double> p = {3, 4, 5, 1};
    auto neighbors = ballTree.knn_search(p, 4);
    for(auto const& neigh: neighbors){
        std::cout << *neigh.first << " " << neigh.second << std::endl;
    }
    std::vector<double> distances(data.getSize());

    std::transform(data.begin(), data.end(), distances.begin(),
                   [&p](const std::shared_ptr<Point<double> >& q) {
                        metrics::dist::Euclidean<double> distance;
                       return distance(p, *q);
                   });
    std::sort(distances.begin(), distances.end());
    std::cout << "brute force dist: "<< distances[0] << " " << distances[1] << " "<< distances[2] << " "<< distances[3] << " "<< std::endl;
}