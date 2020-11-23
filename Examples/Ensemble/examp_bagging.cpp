//
// Created by mateuscmarim on 19/11/2020.
//

#include <chrono>
#include <iostream>
#include "BaggingClassifier.hpp"
#include "Validation.hpp"
#include "PerceptronCommittee.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]){
    mltk::Data<double> data("iris.data");
    mltk::BalancedPerceptron<double> bp;
    mltk::Validation<double> validation;

    for(auto c: data.getClasses()){
        std::cout << c << std::endl;
    }

    auto t1 = high_resolution_clock::now();
    mltk::BaggingClassifier<double> bag(data, bp, 20);
    bag.train();

    validation.setSeed(42);
    validation.setVerbose(2);
    validation.setSamples(mltk::make_data<double>(data));
    validation.setClassifier(&bag);

    auto pred = bag.evaluate((*data[10]));
    std::cout << "Original class: " << data[10]->Y() << "\nPredicted class: " << pred << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(bag, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    auto time = duration_cast<duration<double> >(high_resolution_clock::now() - t1).count();
    std::cout << "Execution time: " << time << " seconds." << std::endl;
    return 0;
}