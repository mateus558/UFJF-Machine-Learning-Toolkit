#include <iostream>
#include "Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsAll.hpp"
#include "IMA.hpp"

int main(int argc, char *argv[]){
    mltk::DataPointer<double> data = std::make_shared<mltk::Data< double > >();
    mltk::Validation<double> validation;
    
    data->setClassesAtEnd(false);
    data->load("iris_mult.csv");
    
    std::cout << *data << std::endl;
    
    mltk::IMAp<double> imap;
    
    imap.setAlphaAprox(1);
    imap.setVerbose(0);
    imap.setFlexible(0.001);
    imap.setMaxTime(110);
    
    mltk::OneVsAll<double, mltk::IMAp> ova(data, std::make_shared<mltk::IMAp<double> >(imap));
    
    ova.train();
    
    std::cout << "Original class: " << (*data)[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ova.evaluate(*(*data)[0]) << std::endl;
    
    validation.setVerbose(2);
    validation.setSamples(data);
    validation.setClassifier(&ova);
    
    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(ova, *data);
    double errors = 0;
    for(size_t i = 0; i < conf_matrix.size(); i++){
        for(size_t j = 0; j < conf_matrix[i].size(); j++){
            if(i != j){
                errors += conf_matrix[i][j];
            }
        }
    }
    
    for(auto& line: conf_matrix){
        for(size_t i = 0; i < line.size(); i++){
            std::cout << line[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Acurracy: " << 1 - errors/data->getSize() << std::endl;


    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    
    return 0;
}