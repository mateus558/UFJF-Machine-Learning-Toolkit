#include <iostream>
#include "Data.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;

    data.setClassesAtEnd(false);
    data.load("iris.data");

    std::cout << "Dataset size: " << data.getSize() << std::endl;
    std::cout << "Dataset dimension: " << data.getDim() << std::endl;
    std::cout << std::endl;

//    for(auto it = data.begin(); it != data.begin()+5; it++){
//        std::cout << *(*it) << std::endl;
//    }
//    std::cout << std::endl;
//    for(auto it = data.begin(); it != data.begin()+5; it++){
//        std::cout << (*it)->Y() << std::endl;
//    }
    std::cout << std::endl;

    auto distribution = data.getClassesDistribution();
    for(auto dist: distribution){
        std::cout << dist << std::endl;
    }
    std::cout << std::endl;

    auto class_names = data.getClasses();
    for(auto class_name: class_names){
        std::cout << class_name << std::endl;
    }

    data.write("iris_teste", "data");

    auto classes_split = data.splitByClasses();
    for(auto &class_split: classes_split){
        if(class_split[0]->Y() == 2)
            std::cout << class_split << std::endl;
    }
    std::cout << std::endl;

    auto samples_split = data.splitSample(10);
    for(auto &sample: samples_split){
        std::cout << sample.getSize() << std::endl;
    }

    std::cout << data.sampling(50, true) << std::endl;
}