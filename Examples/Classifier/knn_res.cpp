//
// Created by mateuscmarim on 12/01/2021.
//

#include <iostream>
#include "../../Modules/Validation/Validation.hpp"
#include "KNNClassifier.hpp"
#include <fstream>
#include <map>

#define DATASET_FOLDER "../datasets/"

using namespace mltk;
using namespace mltk::metrics;

template<typename Metric = dist::Euclidean<double>>
void add_knn(std::vector<std::string> const& datasets, std::vector<size_t> const& ks, std::map<std::string, std::vector<std::pair<double, size_t>>>& res, bool at_end[]){
    Metric metric;
    size_t i = 0;

    std::clog << "\nMetric: " << metric.name() << std::endl;
    std::clog << "Family: " << metric.family() << std::endl;
    std::clog << "\n---------------------------------------------\n";
    for(auto const& dataset: datasets){
        Data<double> data(std::string(DATASET_FOLDER) + dataset, at_end[i]);
        std::clog << "\nDataset: " << dataset << std::endl;
        std::clog << "Size: " <<data.getSize() << std::endl;
        std::clog << "Dimensions: " << data.getDim() << "\n" << std::endl;

        for(auto const& k: ks){
            classifier::KNNClassifier<double, Metric> knn(data, k);

            auto error = validation::kkfold(data, knn, 10, 10, 42, 0).accuracy;
            std::clog << "Accuracy for k = " << k << ": " << error << std::endl;

            res[dataset].push_back(std::make_pair(error, k));
        }
        std::clog << "\n---------------------------------------------\n";
        i++;
    }
    std::clog << "\n---------------------------------------------\n";
}

int main(int argc, char* argv[]){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff", "epileptic.csv"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true,false};
    std::vector<size_t> ks = {3, 5, 7};
    // output for each k value
    std::vector<std::ofstream> results(ks.size());
    std::map<std::string, std::vector<std::pair<double, size_t>>> knns;

    size_t k = 0;
    for(auto& res: results){
        res.open(std::string("result_k_")+std::to_string(ks[k])+".csv");
        if(!res.is_open()){
            std::cerr << "Error opening results file.\n";
            return 1;
        }
        res << "Dataset, Euc , Lor, Cos, Bhatt, Pear, Kull, Hass\n";
        k++;
    }

    add_knn<dist::Euclidean<double>>(datasets, ks, knns, at_end);
    add_knn<dist::Lorentzian<double>>(datasets, ks, knns, at_end);
    add_knn<dist::Cosine<double>>(datasets, ks, knns, at_end);
    add_knn<dist::Bhattacharyya<double>>(datasets, ks, knns, at_end);
    add_knn<dist::Pearson<double>>(datasets, ks, knns, at_end);
    add_knn<dist::KullbackLeibler<double>>(datasets, ks, knns, at_end);
    add_knn<dist::Hassanat<double>>(datasets, ks, knns, at_end);

    for(auto & knn : knns){
        for(auto& res: results){
            res << knn.first << ", ";
        }
        for(auto & i : knn.second){
            auto out_res = std::find(ks.begin(), ks.end(), i.second) - ks.begin();
            results[out_res] << i.first << ", ";
        }
        for(auto& res: results){
            res << "\n";
        }
    }

    for(auto& res: results){
        res.close();
    }
    return 0;
}