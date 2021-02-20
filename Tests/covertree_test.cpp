//
// Created by mateuscmarim on 07/01/2021.
//

#include <chrono>  // for high_resolution_clock
#include "../Modules/Core/Core.hpp"
#include "../Modules/Validation/Validation.hpp"
#include "../Modules/Classifier/Classifier.hpp"

using namespace mltk;

double measure_accuracy(Data<double>& data, classifier::Classifier<double>& classifier){
    double acertos = 0;
    for(auto const& p: data){
        if(classifier.evaluate(*p) == p->Y()){
            acertos++;
        }
    }
    return acertos/data.getSize()*100.0;
}

int main(int argc, char* argv[]){
    Data<double> data("epileptic.csv");
//    metrics::CoverTree<double, std::shared_ptr<Point<double>>, metrics::dist::Hassanat<double>> cover;
//
//    std::cout << "Initializing cover tree" << std::endl;
//    for(const auto& point: data.getPoints()){
//        //Point<double> p(*point);
//        cover.insert(point);
//    }
//    std::cout << "Querying neighbors." << std::endl;
//    auto neigh = cover.kNearestNeighbors(data[0], 7);
//    for(auto const& p: neigh){
//        std::cout << *p << std::endl;
//    }

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Training knn with cover tree" << std::endl;
    classifier::KNNClassifier<double, metrics::dist::Hassanat<double>> knn(data, 3, "covertree");
    knn.train();

    std::cout << "Validating knn model." << std::endl;


    auto acc = measure_accuracy(data, knn);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "Accuracy: " << acc << "%\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n\n\n";
    std::cout << "Training knn with brute force" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    classifier::KNNClassifier<double, metrics::dist::Hassanat<double>> knn_brute(data, 3);
    knn_brute.train();

    std::cout << "Validating knn model." << std::endl;
    acc = measure_accuracy(data, knn_brute);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "Accuracy: " << acc << "%\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}