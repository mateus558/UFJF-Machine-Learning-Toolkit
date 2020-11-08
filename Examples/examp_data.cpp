#include <iostream>
#include <Data.hpp>

int main(int argc, char *argv[]){
    mltk::Data<double> data;

    data.setClassesAtEnd(false);
    data.load("iris_teste.data");

    std::cout << "Dataset size: " << data.getSize() << std::endl;
    std::cout << "Dataset dimension: " << data.getDim() << std::endl;
    std::cout << std::endl;

    for(auto it = data.begin(); it != data.begin()+5; it++){
        std::cout << *(*it) << std::endl;
    }
    std::cout << std::endl;
    for(auto it = data.begin(); it != data.begin()+5; it++){
        std::cout << (*it)->Y() << std::endl;
    }
    std::cout << std::endl;
    auto distribution = data.getClassesDistribution();
    
    for(auto dist: distribution){
        std::cout << dist << std::endl;
    }

    auto class_names = data.getClassNames();
    for(auto class_name: class_names){
        std::cout << class_name << std::endl;
    }
    data.write("iris_teste", "data");
}