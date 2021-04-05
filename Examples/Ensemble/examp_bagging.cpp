//
// Created by mateuscmarim on 19/11/2020.
//

#include <chrono>
#include <iostream>
#include "../Ensemble.hpp"
#include "../../Modules/Validation/Validation.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]){
    mltk::Data<double> data("../iris.data");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    mltk::classifier::BalancedPerceptron<double> bp;

    auto t1 = high_resolution_clock::now();
    mltk::ensemble::BaggingClassifier<double> bag(data, bp, 20);
    bag.train();

    auto pred = bag.evaluate(*data[10]);
    std::cout << "Original class: " << data[10]->Y() << "\nPredicted class: " << pred << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, bag);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();

    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, bag, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    auto time = duration_cast<duration<double> >(high_resolution_clock::now() - t1).count();
    std::cout << "Execution time: " << time << " seconds." << std::endl;
    return 0;
}