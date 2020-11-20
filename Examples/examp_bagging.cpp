//
// Created by mateuscmarim on 19/11/2020.
//

#include <chrono>
#include <iostream>
#include "BaggingClassifier.hpp"
#include "Validation.hpp"
#include "IMA.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]){
    mltk::Data<double> data;
    mltk::IMAp<double> ima;
    mltk::Validation<double> validation;
    //data.setClassesAtEnd(true);
    data.load("iris.data");

    for(auto c: data.getClasses()){
        std::cout << c << std::endl;
    }

    ima.setAlphaAprox(1);
    ima.setVerbose(0);
    ima.setFlexible(0.001);
    ima.setMaxTime(110);

    auto t1 = high_resolution_clock::now();
    mltk::BaggingClassifier<double> voter(data, ima, 10);
    voter.train();

    validation.setVerbose(2);
    validation.setSamples(mltk::make_data<double>(data));
    validation.setClassifier(&voter);

    auto pred = voter.evaluate((*data[100]));
    std::cout << "Original class: " << data[100]->Y() << "\nPredicted class: " << pred << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(voter, data);
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