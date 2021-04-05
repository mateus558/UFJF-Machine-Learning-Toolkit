//
// Created by mateuscmarim on 22/12/2020.
//

#include "RFE.hpp"
#include "IMA.hpp"

int main(){
    mltk::Data<double> data("../iris.data");
    std::cout << data << std::endl;
    mltk::classifier::IMAp<double> ima;
    mltk::featselect::RFE<double> rfe(mltk::make_data<double>(data), &ima, nullptr, 3, 0);

    auto new_data = rfe.selectFeatures();

    std::cout << *new_data << std::endl;
}