//
// Created by mateuscmarim on 22/12/2020.
//

#include "Fisher.hpp"
#include "IMA.hpp"

int main(){
    mltk::Data<double> data("../iris.data");
    std::cout << data << std::endl;
    mltk::classifier::IMAp<double> ima;
    mltk::featselect::Fisher<double> fisher(mltk::make_data<double>(data), &ima, 3);

    auto new_data = fisher.selectFeatures();

    std::cout << *new_data << std::endl;
}