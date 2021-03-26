//
// Created by mateuscmarim on 26/02/2021.
//

#include <kNNEnsembleWPDSM.hpp>
#include "../Modules/Core/Core.hpp"
#include "../Modules/Ensemble/include/kNNEnsembleDSM.hpp"
#include "../Modules/Validation/Validation.hpp"

using namespace mltk;

int main(int argc, char *argv[]){
    Data<double> data("bupa.data");
    DSM<double> dsm(data, 1, data.getDim()*0.5, 1);
    Point<double> p = linspace<double>(0.0, 1.0, 11);
    std::cout << p << std::endl;
    // ensemble::kNNEnsembleWPDSM<double> knn_ensemb(data, 5, 0.1);
//    auto S = dsm(data);
//
//    for(auto& Si: S){
//        for(auto feat: Si){
//            std::cout << feat << " ";
//        }
//        std::cout << std::endl;
//    }
    //auto acc = validation::kkfold(data, knn_ensemb, 10, 10, 0, 1).accuracy;
    //std::cout << "Accuracy: " << acc << std::endl;

}