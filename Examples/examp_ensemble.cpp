#include <iostream>
#include <chrono>
#include "VotingClassifier.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"
#include "OneVsAll.hpp"
#include "Validation.hpp"
#include "PerceptronCommittee.hpp"

using namespace std::chrono;

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    mltk::IMAp<double> ima;
    mltk::PerceptronPrimal<double> perc;
    mltk::KNNClassifier<double> knn(30);
    std::vector<double> weights = {1.5, 1, 2};

    //data.setClassesAtEnd(true);
    data.load("iris_mult.csv");

    for(auto c: data.getClasses()){
        std::cout << c << std::endl;
    }
    
    ima.setAlphaAprox(1);
    ima.setVerbose(0);
    ima.setFlexible(0.001);
    ima.setMaxTime(110);

    mltk::PerceptronCommittee<double> pc(data, 20);
    mltk::OneVsAll<double> ova_ima(data, ima);
    mltk::OneVsAll<double> ova_pc(data, pc);

    auto t1 = high_resolution_clock::now();
    mltk::VotingClassifier<double> voter(data, "soft", knn, ova_ima, ova_pc);
    voter.setWeights(weights);
    voter.train();

    auto pred = voter.evaluate(*data[100]);
    std::cout << "Original class: " << data[100]->Y() << "\nPredicted class: " << pred << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(voter, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::Validation<double> validation(data, &voter, 1);
    validation.setVerbose(2);
    mltk::ValidationSolution s = validation.validation(10, 10);
    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    auto time = duration_cast<duration<double> >(high_resolution_clock::now() - t1).count();
    std::cout << "Execution time: " << time << " seconds." << std::endl;
}