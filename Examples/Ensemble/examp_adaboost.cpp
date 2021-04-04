//
// Created by mateuscmarim on 20/11/2020.
//

#include "AdaBoostClassifier.hpp"
#include "PerceptronCommittee.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include "OneVsAll.hpp"

int main(int argc, char* argv[]){
    mltk::Data<double> data("../iris_mult.csv");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    mltk::classifier::BalancedPerceptron<double> bp(data);
    bp.setMaxTime(2500);
    mltk::classifier::OneVsAll<double> ova(valid_pair.train, bp);
    mltk::ensemble::AdaBoostClassifier<double> ada(valid_pair.train, ova, 100);
    ada.setSeed(42);

    std::cout << "Dataset size: " << data.size() << std::endl;

    ada.train();
    std::cout << "Original class: " << data[15]->Y() << std::endl;
    std::cout << "Evaluated class: " << ada.evaluate(*data[15]) << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, ada);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();

    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix, true);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, ada, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
}