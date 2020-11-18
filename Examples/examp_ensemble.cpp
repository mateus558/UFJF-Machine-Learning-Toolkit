#include <iostream>
#include "VotingClassifier.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"
#include "OneVsAll.hpp"
#include "Validation.hpp"
#include "PerceptronCommittee.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    mltk::IMAp<double> ima;
    mltk::PerceptronPrimal<double> perc;
    mltk::KNNClassifier<double> knn(30);
    std::vector<double> weights = {2, 1};
    mltk::Validation<double> validation;
    
    //data.setClassesAtEnd(true);
    data.load("iris_mult.csv");

    for(auto c: data.getClasses()){
        std::cout << c << std::endl;
    }
    
    ima.setAlphaAprox(1);
    ima.setVerbose(0);
    ima.setFlexible(0.001);
    ima.setMaxTime(110);
    
    mltk::OneVsAll<double> ova(std::make_shared<mltk::Data< double > >(data), ima);
    mltk::PerceptronCommittee<double> pc(data, 20);
    mltk::VotingClassifier<double> voter(data, "soft", knn, ova, pc);
    
    
    voter.setWeights(weights);
    voter.train();

    validation.setVerbose(2);
    validation.setSamples(std::make_shared<mltk::Data< double > >(data));
    validation.setClassifier(&voter);

    std::cout << "Original class: " << data[100]->Y() << "\nPredicted class: " << voter.evaluate(*data[100]) << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(voter, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
}