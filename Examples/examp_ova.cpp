#include <iostream>
#include "Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsAll.hpp"
#include "IMA.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    
    data.setClassesAtEnd(false);
    data.load("breast.data");
    
    //std::cout << *data << std::endl;
    
    mltk::IMAp<double> imap;
    
    imap.setAlphaAprox(1);
    imap.setVerbose(0);
    imap.setFlexible(0.001);
    imap.setMaxTime(110);
    
    mltk::OneVsAll<double> ova(data, imap);
    
    ova.train();
    
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ova.evaluate(*data[0]) << std::endl;

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ova, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::Validation<double> validation(data, &ova, 1);
    validation.setVerbose(2);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    
    return 0;
}