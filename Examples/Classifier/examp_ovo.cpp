#include <iostream>
#include "../../Modules/Validation/Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsOne.hpp"
#include "IMA.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data("../iris_mult.csv");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    mltk::classifier::IMAp<double> imap;
    imap.setVerbose(0);
    imap.setFlexible(0.001);

    mltk::classifier::OneVsOne<double> ovo(valid_pair.train, imap);
    std::cout << data << std::endl;
    ovo.train();
 
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ovo.evaluate(*data[0]) << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, ovo);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();
    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, ovo, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    return 0;
}