//
// Created by mateuscmarim on 20/11/2020.
//

#include "AdaBoostClassifier.hpp"
#include "PerceptronCommittee.hpp"
#include "Validation.hpp"
#include "OneVsAll.hpp"

int main(int argc, char* argv[]){
    mltk::Data<double> data("letter-recognition.csv");
    mltk::BalancedPerceptron<double> bp(data);
    bp.setMaxTime(2500);
    mltk::OneVsAll<double> ova(data, bp);

    mltk::Validation<double> validation(data);
    validation.setVerbose(2);
    validation.partTrainTest(5);

   // mltk::ensemble::AdaBoostClassifier<double> ada(*validation.getTrainSample(), bp, 100);
    //ada.setSeed(42);
    ova.train();
    std::cout << "Original class: " << data[15]->Y() << std::endl;
    std::cout << "Evaluated class: " << ova.evaluate(*data[15]) << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ova, *validation.getTestSample());
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    validation.setClassifier(&ova);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
}