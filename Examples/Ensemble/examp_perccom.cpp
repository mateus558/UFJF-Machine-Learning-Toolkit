#include <iostream>
#include "PerceptronCommittee.hpp"
#include "Validation.hpp"
#include "Utils.hpp"
#include <chrono>

int main(int argc, char *argv[]){
    mltk::Data<double> _data, data;

    //_data.setClassesAtEnd(true);
    _data.load("iris.data");
    std::cout << _data.getSize() << std::endl;
    std::vector<int> cl = {-1,1};
    data.setClasses(cl);
    data.classesCopy(_data, cl);
    auto _classes = data.getClasses();
    for(auto c: _classes){
        std::cout << c << std::endl;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    mltk::PerceptronCommittee<double> bp(data, 100);
    mltk::Validation<double> validation;

    bp.train();

    auto conf_matrix = mltk::Validation<double>::generateConfusionMatrix(bp, data);
    auto classes = data.getClasses();
    mltk::utils::printConfusionMatrix(classes, conf_matrix);
    std::cout << "Error: " << 100.0-mltk::Validation<double>::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;
    
    validation.setVerbose(2);
    validation.setSamples(mltk::make_data<double>(data));
    validation.setClassifier(&bp);
    validation.partTrainTest(10);
    mltk::ValidationSolution s = validation.validation(10, 10);

    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    std::cout << "Time: " << time << " seconds" << std::endl;
    return 0;
}