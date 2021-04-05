//
// Created by mateus on 01/04/2021.
//

#include <iostream>
#include "../Modules/Classifier/include/hnswlib/hnswalg.h"
#include "Data.hpp"
#include "KNNClassifier.hpp"
#include "../Modules/Validation/Validation.hpp"
#include <chrono>

using namespace std::chrono;

int main(int argc, char* argv[]){
    mltk::Data<double> data("wdbc.data");
    mltk::classifier::KNNClassifier<double> knn_hsnw(data, 3, "hsnw");
    mltk::classifier::KNNClassifier<double> knn_brute(data, 3, "brute");

    auto start = high_resolution_clock::now();
    std::cout << "HSNW acc: " << 100.0-mltk::validation::kfold(data, knn_hsnw, 10, 42, 0) << "%" << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::seconds>(stop - start);
// To get the value of duration use the count()
// member function on the duration object
    std::cout << duration.count()<< "s" << std::endl;
    start = high_resolution_clock::now();
    std::cout << "Brute acc: " << 100.0-mltk::validation::kfold(data, knn_brute, 10, 42, 0) << "%" << std::endl;
    stop = high_resolution_clock::now();
    duration = duration_cast<std::chrono::seconds>(stop - start);
    std::cout << duration.count()<<"s" << std::endl;
    //    hnswlib::L2Space space(data.getDim());
//    hnswlib::HierarchicalNSW<float> alg_hnsw(&space, data.getSize());
//
//    int j = 0;
//    for(auto& p : data){
//        alg_hnsw.addPoint(p->X().data(), j);
//        j++;
//    }
//
//    for(auto& p : data){
//        alg_hnsw.addPoint(p->X().data(), j);
//        j++;
//    }
//    auto result = alg_hnsw.searchKnnCloserFirst((const void *) data[1].get(), 3);
//
//    std::cout << *data[1] << std::endl;
//    std::cout << std::endl;
//    for(auto r: result){
//        std::cout << *data[r.second] << std::endl;
//    }

}