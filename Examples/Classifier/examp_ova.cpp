#include <iostream>
#include "../../Modules/Validation/Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsAll.hpp"
#include "IMA.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data("../letter-recognition.csv");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    mltk::classifier::IMAp<double> imap;
    imap.setVerbose(0);
    imap.setFlexible(300);
    imap.setMaxTime(2500);
    mltk::classifier::OneVsAll<double> ova(valid_pair.train, imap);

    std::cout << data << std::endl;

    ova.train();
    
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ova.evaluate(*data[0]) << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, ova);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();
    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix, true);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, ova, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    
    return 0;
}