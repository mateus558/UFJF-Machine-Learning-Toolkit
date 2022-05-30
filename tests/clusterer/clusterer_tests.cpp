//
// Created by mateus on 16/05/2021.
//
#include <gtest/gtest.h>
#include "ufjfmltk/core/DistanceMetric.hpp"
#include "ufjfmltk/Clusterer.hpp"
#include "ufjfmltk/Validation.hpp"
#include "ufjfmltk/core/Utils.hpp"

class ClustererTest: public ::testing::Test
{
protected:
    void SetUp() override{
        if(!mult.load("iris_mult.csv")){
            std::cerr << "Error loading multiclass dataset." << std::endl;
        }
        if(!bin.load("iris.data")){
            std::cerr << "Error loading binary dataset." << std::endl;
        }
        if(!wine.load("wine.data")){
            std::cerr << "Error loading binary dataset." << std::endl;
        }
    }
    mltk::Data<double> mult, bin, wine;
};

TEST_F(ClustererTest, KMeansTest){
    mltk::clusterer::KMeans<double> kmeans(mult, 3, "random", 0, 1);
    mltk::clusterer::KMeans<double> kmeanspp(mult, 3, "kmeanspp", 0, 1);

    kmeans.setMaxTime(100);
    kmeans.train();
    kmeanspp.setMaxTime(100);
    kmeanspp.train();

    auto centers_rand = kmeans.centers();
    for(const auto& center: centers_rand){
        std::cout << mltk::Point<double>(center) << std::endl;
    }
    std::cout << std::endl;
    auto centerspp = kmeanspp.centers();
    for(const auto& center: centerspp){
        std::cout << mltk::Point<double>(center) << std::endl;
    }
    std::cout << std::endl;
    auto clusters_rand = kmeans.clusters();
    for(const auto& cluster: clusters_rand){
        for(const auto& id: cluster){
            std::cout << *mult[id] << std::endl;
        }
        std::cout << "Elements on cluster: " << cluster.size() << std::endl;
        std::cout << "---------------------END OF CLUSTER---------------------" << std::endl;
    }
    std::cout << std::endl;
    auto clusterspp = kmeanspp.clusters();
    for(const auto& cluster: clusterspp){
        for(const auto& id: cluster){
            std::cout << *mult[id] << std::endl;
        }
        std::cout << "Elements on cluster: " << cluster.size() << std::endl;
        std::cout << "---------------------END OF CLUSTER---------------------" << std::endl;
    }
}
