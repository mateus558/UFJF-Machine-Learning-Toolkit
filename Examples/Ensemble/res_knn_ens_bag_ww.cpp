//
// Created by mateuscmarim on 05/02/2021.
//

#include "kNNEnsembleWSS.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include <map>
#include<tuple> // for tuple

#define DATASET_FOLDER "/home/mateuscmarim/Documents/datasets/"

using namespace mltk;
using namespace mltk::ensemble;

int main(){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true};
    std::vector<size_t> ks = {3, 5, 7};
    std::map<std::string, std::vector<std::tuple<double, size_t, Point<double>>>> knns;
    std::vector<std::ofstream> results(ks.size());

    size_t k = 0;
    for(auto& res: results){
        res.open(std::string("result_k_")+std::to_string(ks[k])+"_ensemb_WSSP_no_zero_11_100.csv");
        if(!res.is_open()){
            std::cerr << "Error opening results file.\n";
            return 1;
        }
        res << "Dataset, ELCBPKH WV SS, Weights\n";
        k++;
    }

    std::clog << "\n---------------------------------------------\n";
    size_t i = 0;
    for(auto& dataset: datasets){
        Data<double> data;
        if(!data.load(DATASET_FOLDER + dataset, at_end[i])) {
            std::cerr << "Error at dataset " << DATASET_FOLDER + dataset << std::endl;
            continue;
        }

        std::clog << "\nDataset: " << dataset << std::endl;
        std::clog << "Size: " << data.getSize() << std::endl;
        std::clog << "Dimensions: " << data.getDim() << "\n" << std::endl;

        for(auto const& _k: ks){
            kNNEnsembleWSS<double> knn_ensem(data, _k);
            knn_ensem.train();
            knn_ensem.optimizeSubWeights(data, 8, 105);
            auto sub_weights = knn_ensem.getSubWeights();
            auto acc = validation::kkfold(data, knn_ensem, 10, 10, 42, 0).accuracy;

            std::clog << "Accuracy for k = " << _k << ": " << acc << std::endl;

            knns[dataset].push_back(std::make_tuple(acc, _k, sub_weights));
        }
        std::clog << "\n---------------------------------------------\n";

        i++;
    }

    for(auto & knn : knns){
        for(auto& res: results){
            res << knn.first << ", ";
        }
        for(auto & out : knn.second){
            auto out_res = std::find(ks.begin(), ks.end(), std::get<1>(out)) - ks.begin();
            auto w = std::get<2>(out);
            results[out_res] << std::get<0>(out) << ", [";
            for(i = 0; i < w.size()-1; i++){
                results[out_res] << w[i] << " | ";
            }
            results[out_res] << w[i] << "]";
        }
        for(auto& res: results){
            res << "\n";
        }
    }

    for(auto& res: results){
        res.close();
    }
}
