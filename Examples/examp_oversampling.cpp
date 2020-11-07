#include "Sampling.hpp"

int main(int argc, char *argv[]){
    mltk::DataPointer<double> data = std::make_shared<mltk::Data< double > >();
    mltk::DataPointer<double> data1 = std::make_shared<mltk::Data< double > >();
    
    data->load("iris_mult.csv");
    data1->copy(*data);
    std::cout << *data << std::endl;

    mltk::SMOTE<double> smote;
    smote(data);

    std::cout << *data << std::endl;

    auto class_distribution = data->getClassesDistribution();
    
    mltk::BorderlineSMOTEOne<double> bsmote1;
    bsmote1(data1);

    std::cout << *data1 << std::endl;

    std::cout << "SMOTE final classes distribution: " << std::endl;
    for(auto &dist: class_distribution){
        std::cout << dist << " ";
    }
    std::cout << '\n' << std::endl;
    
    class_distribution = data1->getClassesDistribution();
    std::cout << "BorderlineSMOTE 1 final classes distribution: " << std::endl;
    for(auto &dist: class_distribution){
        std::cout << dist << " ";
    }
    std::cout << std::endl;


    return 0;
}