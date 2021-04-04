//
// Created by mateuscmarim on 27/01/2021.
//

#include <iostream>
#include "../../Modules/Validation/Validation.hpp"
#include "KNNClassifier.hpp"
#include <fstream>
#include <map>

#define DATASET_FOLDER "../datasets/"

using namespace mltk;
using namespace mltk::metrics;

std::vector<double> ratios = {0.25, 0.5, 0.75, 1};

Data<double> RSM(Data<double> samples, const double ratio, const size_t seed){
    std::random_device rd;
    std::mt19937 generator(rd());
    size_t new_dim = std::ceil(ratio * samples.dim());
    std::vector<size_t> feats(samples.dim());

    std::iota(feats.begin(), feats.end(), 0);
    std::shuffle(feats.begin(), feats.end(), generator);

    std::vector<size_t> new_feats(new_dim);
    for(size_t j = 0; j < new_feats.size(); j++){
        new_feats[j] = feats[j];
    }
    return samples.selectFeatures(new_feats);
}

template<typename Metric = dist::Hassanat<double>>
std::string test_knn_rsm(const Data<double>& data, size_t k){
    classifier::KNNClassifier<double, Metric> knn(k);
    std::string result = ",";
    for(auto const& ratio: ratios){
        auto new_data = RSM(data, ratio, 0);
        knn.setSamples(new_data);
        knn.train();
        auto accuracy = validation::kkfold(new_data, knn, 10, 10, 0, 0).accuracy;
        result += std::to_string(accuracy) + ",";
    }
    result += "\n";
    return result;
}

int main(int argc, char *argv[]){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true, false};
    std::vector<size_t> ks = {3, 5, 7};

    int i = 0;
    for(auto const& k: ks) {
        std::ofstream file("result_knn_rsm_" + std::to_string(k) + "_hassanat.csv");
        if(!file.is_open()){
            std::cerr << "File could not be opened!" << std::endl;
            return 1;
        }
        file << "dataset,";
        for(auto const& ratio: ratios){
            file << "ratio " + std::to_string(ratio) + ", ";
        }
        file << "\n";
        for (auto const &dataset: datasets) {
            Data<double> data(DATASET_FOLDER + dataset, i);
            auto result = dataset + test_knn_rsm(data, k);
            file << result;
            i++;
        }
        file.close();
    }
}