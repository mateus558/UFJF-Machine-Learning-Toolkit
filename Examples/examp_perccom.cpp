#include <iostream>
#include "PerceptronCommittee.hpp"
#include "Validation.hpp"
#include "Utils.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;

    data.setClassesAtEnd(true);
    data.load("iris.data");

    std::cout << data << std::endl;

    mltk::PerceptronCommittee<double> bp(data, 1000);
    mltk::Validation<double> validation;

    bp.train();
    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(bp, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Accuracy: " << mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;
    
    validation.setVerbose(2);
    validation.setSamples(std::make_shared<mltk::Data< double > >(data));
    validation.setClassifier(&bp);
    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    
    return 0;
}