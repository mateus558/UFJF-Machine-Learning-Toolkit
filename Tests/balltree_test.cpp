//
// Created by mateuscmarim on 07/01/2021.
//

#include "../Modules/Core/Core.hpp"

using namespace mltk;

int main(int argc, char* argv[]){
    Data<double> data("iris.data");
    metrics::BallTree<double> ballTree(data);

    std::clog << "Number of nodes: " << metrics::BallTree<double>::nodes_count << std::endl;
    std::clog << "Dataset: breast\nSize: " << data.getSize() << "\nDimensions: " << data.getDim() << std::endl;
    std::clog << std::endl;
}