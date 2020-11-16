#include <iostream>
#include "VotingClassifier.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    mltk::IMAp<double> ima;
    mltk::PerceptronPrimal<double> perc;
    mltk::KNNClassifier<double> knn(30);
    data.load("iris.data");

    for(auto c: data.getClasses()){
        std::cout << c << std::endl;
    }
    ima.setVerbose(0);
    mltk::VotingClassifier<double> voter(data, "soft", knn, ima);
    std::vector<double> weights = {0.35, 1};
    voter.setWeights(weights);
    voter.train();
    std::cout << "Original class: " << data[30]->Y() << "\nPredicted class: " << voter.evaluate(*data[30]) << std::endl;
}