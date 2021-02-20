//
// Created by mateuscmarim on 25/01/2021.
//

#include "kNNEnsembleWRSM.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include <map>

#define DATASET_FOLDER "../datasets/"

using namespace mltk;
using namespace mltk::ensemble;

int main(){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff", "epileptic.csv"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true,false};
    std::vector<size_t> ks = {3, 5, 7};
    std::map<std::string, std::vector<std::pair<double, size_t>>> knns;
    std::vector<std::ofstream> results(ks.size());

    size_t k = 0;
    for(auto& res: results){
        res.open(std::string("result_k_")+std::to_string(ks[k])+"_ensemb_rsm.csv");
        if(!res.is_open()){
            std::cerr << "Error opening results file.\n";
            return 1;
        }
        res << "Dataset, ELCBPKH HV bagging\n";
        k++;
    }


    std::clog << "\n---------------------------------------------\n";
    size_t i = 0;
    for(auto& dataset: datasets){
        Data<double> data(std::string(DATASET_FOLDER) + dataset, at_end[i]);
        std::clog << "\nDataset: " << dataset << std::endl;
        std::clog << "Size: " <<data.getSize() << std::endl;
        std::clog << "Dimensions: " << data.getDim() << "\n" << std::endl;

        for(auto const& k: ks){
            kNNEnsembleBaggingWRSM<double> knn_ensem(data, k, 0.5);
            knn_ensem.train();

            auto error = validation::kkfold(data, knn_ensem, 10, 10, 42, 0).accuracy;
            std::clog << "Accuracy for k = " << k << ": " << error << std::endl;

            knns[dataset].push_back(std::make_pair(error, k));
        }
        std::clog << "\n---------------------------------------------\n";

        i++;
    }

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
}