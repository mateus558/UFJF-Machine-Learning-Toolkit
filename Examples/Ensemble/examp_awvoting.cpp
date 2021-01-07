//
// Created by mateuscmarim on 05/01/2021.
//

#include "AutoWeightedVoting.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"
#include "AdaBoostClassifier.hpp"
#include "PerceptronCommittee.hpp"
#include "../../Modules/Validation/Validation.hpp"

int main(int argc, char* argv[]){
    mltk::Data<double> data("../wdbc.data");
    mltk::classifier::IMAp<double> ima;
    mltk::ensemble::AdaBoostClassifier<double> ada;
    mltk::classifier::BalancedPerceptron<double> bp(data);
    ima.setVerbose(0);

    mltk::classifier::KNNClassifier<double> knn(data, 3);
    bp.setSeed(0);
    mltk::ensemble::AutoWeightedVoting<double> awvote(data, bp, knn);
    awvote.setMaxIterations(15);

    knn.train();
    std::cout << "KNN accuracy: " << 100 - mltk::validation::kfold(data, knn, 10, 0, 0) << std::endl;
    ima.setSamples(data);
    ima.train();
    std::cout << "BP accuracy: " << 100 - mltk::validation::kfold(data, bp, 10, 0, 0) << std::endl;
    awvote.train();
    //std::cout << awvote.simplex_projection(y) << std::endl;
    //std::cout << awvote.simplex_projection_c(y, 0, 1, 1E-10) << std::endl;
}
