//
// Created by mateuscmarim on 22/12/2020.
//

#include "Data.hpp"

#include <iostream>

int main(int, char**){
    mltk::Data<double> data("../iris.data");

    std::cout << data.size() << std::endl;

    return 0;
}