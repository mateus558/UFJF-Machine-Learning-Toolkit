#include <iostream>
#include "Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsOne.hpp"
#include "IMA.hpp"
#include "Sampling.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    mltk::Validation<double> validation;
    
    //data->setClassesAtEnd(true);
    data.load("iris_mult.csv");
    
    std::cout << data << std::endl;
    
    mltk::IMAp<double> imap;
    
    imap.setAlphaAprox(1);
    imap.setVerbose(0);
    imap.setFlexible(0.001);
    imap.setMaxTime(110);
    
    mltk::SMOTE<double> smote;
    mltk::OneVsOne<double> ovo(data, imap, &smote);
    
    ovo.train();
    
    std::cout << "Original class: " << data[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ovo.evaluate(*data[0]) << std::endl;
    
    validation.setVerbose(2);
    validation.setSamples(data);
    validation.setClassifier(&ovo);
    
    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ovo, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    
    return 0;
}