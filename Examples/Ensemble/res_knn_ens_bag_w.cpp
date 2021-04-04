//
// Created by mateuscmarim on 05/02/2021.
//

#include "kNNEnsembleW.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include <map>

#define DATASET_FOLDER "/home/mateuscmarim/Documents/datasets/"

using namespace mltk;
using namespace mltk::ensemble;

int main(){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true};
    std::vector<size_t> ks = {3, 5, 7};
    std::map<std::string, std::vector<std::pair<double, size_t>>> knns;
    std::vector<std::ofstream> results(ks.size());

    size_t k = 0;
    for(auto& res: results){
        res.open(std::string("result_k_")+std::to_string(ks[k])+"_ensemb_W.csv");
        if(!res.is_open()){
            std::cerr << "Error opening results file.\n";
            return 1;
        }
        res << "Dataset, ELCBPKH WV Bagging\n";
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
        std::clog << "Size: " << data.size() << std::endl;
        std::clog << "Dimensions: " << data.dim() << "\n" << std::endl;
        auto train_test = validation::partTrainTest(data, 10, 42);
        for(auto const& k: ks){
            kNNEnsembleBaggingW<double> knn_ensem(data, k);
            knn_ensem.train();
            auto error = validation::kkfold(data, knn_ensem, 10, 10, 42, 0).accuracy;
            //std::clog << "Max accuracy: " << knn_ensem.maxAccuracy() << std::endl;
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
