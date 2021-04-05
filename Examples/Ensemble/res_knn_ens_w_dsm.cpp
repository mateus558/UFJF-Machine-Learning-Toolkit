//
// Created by mateuscmarim on 25/01/2021.
//

#include "kNNEnsembleWPDSM.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include <map>

#define DATASET_FOLDER "../datasets/"

using namespace mltk;
using namespace mltk::ensemble;

int main(){
    std::vector<std::string> datasets = {/*"pima.data", "sonar.data", "bupa.data","wdbc.data", "ionosphere.data", "biodegradetion.csv",
            "vehicle.csv",*/ "seismic-bumps.arff"};
    bool at_end[10] = {true};
    std::vector<size_t> ks = {3, 5, 7};
    std::vector<double> alphas = {0, 0.1, 0.2,0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
    std::map<std::string, std::vector<std::tuple<double, size_t, double>>> knns;
    std::vector<std::vector<std::ofstream>> results(ks.size());

    size_t k = 0;
    for(auto & result : results){
        result.resize(alphas.size());
        size_t i = 0;
        for(auto alpha:alphas) {
            result[i].open(std::string("result_k_") + std::to_string(ks[k]) + "_r_"+std::to_string(alpha)+"ensemb_bag_wpdsm.csv");
            if (!result[i].is_open()) {
                std::cerr << "Error opening results file.\n";
                return 1;
            }
            result[i] << "Dataset, ELCBPKH bag DSM\n";
            i++;
        }
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
            for(int ai = 0; ai < alphas.size(); ai++)
                res[ai] << dataset << ", ";
        }

        for(auto const& _k: ks){
            double best_alpha = 0.0;
            double best_acc = 0.0;
            for(auto const& alpha: alphas) {
                kNNEnsembleWPDSM<double> knn_ensem(data, _k, alpha);
                knn_ensem.train();

                auto acc = validation::kkfold(data, knn_ensem, 10, 10, 42, 0).accuracy;
                std::clog << "Accuracy for _k = " << _k << " and alpha = " << alpha << ": " << acc << std::endl;
                if(acc > best_acc){
                    best_acc = acc;
                    best_alpha = alpha;
                }
                auto out_kline = std::find(ks.begin(), ks.end(), _k) - ks.begin();
                auto out_alphacol = std::find(alphas.begin(), alphas.end(), alpha) - alphas.begin();
                results[out_kline][out_alphacol] << acc << ", ";
                knns[dataset].push_back(std::make_tuple(acc, _k, alpha));
            }
            std::cout << "K= " << _k << "; BEST ALPHA = " << best_alpha << "; BEST ACC = " << best_acc << std::endl;
            kNNEnsembleWPDSM<double> knn_ensem(data, _k, best_alpha);
            knn_ensem.train();
            knn_ensem.optimizeSubWeights(data, 9, 104);
        }
        std::clog << "\n---------------------------------------------\n";
        for(auto& res: results){
            for(int ai = 0; ai < alphas.size(); ai++)
                res[ai] << "\n";
        }
        i++;
    }

    for(auto& res: results){
        for(int ai = 0; ai < alphas.size(); ai++)
            res[ai].close();
    }
}