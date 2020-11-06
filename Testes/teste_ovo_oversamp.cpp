#include <iostream>
#include "Validation.hpp"
#include "Perceptron.hpp"
#include "SMO.hpp"
#include "OneVsOne.hpp"
#include "IMA.hpp"
#include "Sampling.hpp"

int main(int argc, char *argv[]){
    DataPointer<double> data = std::make_shared<Data< double > >();
    Validation<double> validation;
    
    //data->setClassesAtEnd(true);
    data->load("iris_mult.csv");
    
    std::cout << *data << std::endl;
    
    IMAp<double> imap;
    
    imap.setAlphaAprox(1);
    imap.setVerbose(0);
    imap.setFlexible(0.001);
    imap.setMaxTime(110);
    
    SMOTE<double> smote;
    OneVsOne<double, IMAp> ovo(data, std::make_shared<IMAp<double> >(imap), &smote);
    
    ovo.train();
    
    std::cout << "Original class: " << (*data)[0]->Y() << std::endl;
    std::cout << "Evaluated class: " << ovo.evaluate(*(*data)[0]) << std::endl;
    
    validation.setVerbose(2);
    validation.setSamples(data);
    validation.setClassifier(&ovo);
    
    auto conf_matrix = Validation<double>::generateConfusionMatrix(ovo, *data);
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
    
    validation.partTrainTest(10);
    ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << std::endl;
    std::cout << "Acurracy: " << 1 - errors/data->getSize() << std::endl;
    
    return 0;
}