#include <iostream>
#include <chrono>
#include "VotingClassifier.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"
#include "OneVsAll.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include "PerceptronCommittee.hpp"

using namespace std::chrono;

int main(int argc, char *argv[]){
    mltk::Data<double> data("../iris_mult.csv");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    std::vector<double> weights = {1.5, 1, 2};

    auto t1 = high_resolution_clock::now();
    mltk::classifier::PerceptronPrimal<double> perc;
    mltk::classifier::IMAp<double> ima;
    ima.setVerbose(0);
    ima.setFlexible(0.001);
    mltk::classifier::KNNClassifier<double> knn(data, 30);
    mltk::ensemble::PerceptronCommittee<double> pc(data, 20);
    mltk::classifier::OneVsAll<double> ova_ima(data, ima);
    mltk::classifier::OneVsAll<double> ova_pc(data, pc);
    mltk::ensemble::VotingClassifier<double> voter(valid_pair.train, "soft", knn, ova_ima, ova_pc);

    voter.setWeights(weights);
    voter.train();
    auto pred = voter.evaluate(*data[100]);
    std::cout << "Original class: " << data[100]->Y() << "\nPredicted class: " << pred << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, voter);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();

    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, voter, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    auto time = duration_cast<duration<double> >(high_resolution_clock::now() - t1).count();
    std::cout << "Execution time: " << time << " seconds." << std::endl;
}