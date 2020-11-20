#include <iostream>
#include "Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsOne.hpp"
#include "IMA.hpp"
#include "Sampling.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data("iris_mult.csv");
    std::cout << data << std::endl;
    
    mltk::IMAp<double> imap;
    imap.setVerbose(0);
    imap.setFlexible(0.001);

    mltk::SMOTE<double> smote;
    mltk::OneVsOne<double> ovo(data, imap, &smote);
    ovo.train();
    
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ovo.evaluate(*data[0]) << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ovo, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::Validation<double> validation(data, &ovo, 1);
    validation.setVerbose(2);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    
    return 0;
}