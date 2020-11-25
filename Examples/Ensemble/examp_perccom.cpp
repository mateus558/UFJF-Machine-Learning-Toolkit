#include <iostream>
#include "PerceptronCommittee.hpp"
#include "../../Modules/Validation/Validation.hpp"
#include "Utils.hpp"
#include <chrono>

int main(int argc, char *argv[]){
    mltk::Data<double> data("../iris.data");
    auto valid_pair = mltk::validation::partTrainTest(data, 10, 0);
    auto t1 = std::chrono::high_resolution_clock::now();
    mltk::ensemble::PerceptronCommittee<double> pc(valid_pair.train, 100);

    pc.train();

    std::cout << "Original class: " << data[15]->Y() << std::endl;
    std::cout << "Evaluated class: " << pc.evaluate(*data[15]) << std::endl;

    auto conf_matrix = mltk::validation::generateConfusionMatrix(valid_pair.test, pc);
    auto classes = data.getClasses();
    auto classes_names = data.getClassNames();

    mltk::utils::printConfusionMatrix(classes, classes_names, conf_matrix, true);
    std::cout << "Error: " << 100.0-mltk::validation::confusionMatrixAccuracy(conf_matrix) << "%" << std::endl;

    mltk::validation::ValidationSolution s = mltk::validation::kkfold(data, pc, 10, 10, 42, 2);
    std::cout << "Validation accuracy: " << s.accuracy << "%" << std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    std::cout << "Time: " << time << " seconds" << std::endl;

    return 0;
}