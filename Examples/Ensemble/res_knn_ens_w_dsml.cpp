//
// Created by mateuscmarim on 25/01/2021.
//

#include "kNNEnsembleWPDSMLinear.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include <map>

#define DATASET_FOLDER "/fast/Projects/UFJF-MLTK/UFJF-MLTK/release/Examples/datasets/"

using namespace mltk;
using namespace mltk::ensemble;

int main(){
    std::vector<std::string> datasets = {"ThoraricSurgery.arff","vehicle.csv", "seismic-bumps.arff"};
    bool at_end[10] = {true,false, true};
    std::vector<size_t> ks = {3, 5, 7};
    std::map<std::string, std::vector<std::tuple<double, size_t>>> knns;
    std::vector<std::ofstream> results(ks.size());

    size_t k = 0;
    for(auto & result : results){
        size_t i = 0;
        result.open(std::string("result_k_") + std::to_string(ks[k]) + "_ensemb_wpdsm_linear.csv");
        if (!result.is_open()) {
            std::cerr << "Error opening results file.\n";
            return 1;
        }
        result << "Dataset, ELCBPKH DSM Linear\n";
        i++;
        k++;
    }


    std::clog << "\n---------------------------------------------\n";
    size_t i = 0;
    for(auto& dataset: datasets){
        Data<double> data(std::string(DATASET_FOLDER) + dataset, at_end[i]);
        std::clog << "\nDataset: " << dataset << std::endl;
        std::clog << "Size: " << data.size() << std::endl;
        std::clog << "Dimensions: " << data.dim() << "\n" << std::endl;

        for(auto& res: results){
            res << dataset << ", ";
        }

        for(auto const& _k: ks){
                kNNEnsembleWPDSMLinear<double> knn_ensem(data, _k);
                knn_ensem.train();
                knn_ensem.optimizeSubWeights(data, 9, 104);
                auto acc = validation::kkfold(data, knn_ensem, 10, 10, 42, 0).accuracy;
                std::clog << "Accuracy for _k = " << _k << ": " << acc << std::endl;
                std::clog << "Sub weights: " << knn_ensem.getSubWeights() << std::endl;
                auto out_kline = std::find(ks.begin(), ks.end(), _k) - ks.begin();
                results[out_kline] << acc << ", ";
                knns[dataset].push_back(std::make_tuple(acc, _k));
        }
        std::clog << "\n---------------------------------------------\n";
        for(auto& res: results){
                res << "\n";
        }
        i++;
    }

    for(auto& res: results){
        res.close();
    }
}