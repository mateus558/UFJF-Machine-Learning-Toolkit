//
// Created by mateuscmarim on 20/11/2020.
//

#include "AdaBoostClassifier.hpp"
#include "PerceptronCommittee.hpp"
#include "Validation.hpp"

int main(int argc, char* argv[]){
    mltk::Data<double> data("iris.data");
    mltk::BalancedPerceptron<double> bp;
    bp.setMaxTime(300);
    mltk::ensemble::AdaBoostClassifier<double> ada(data, bp, 20);
    ada.setSeed(42);
    ada.train();
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ada.evaluate(*data[0]) << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ada, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::Validation<double> validation(data, &ada, 1);
    validation.setVerbose(2);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
}