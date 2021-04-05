//
// Created by mateuscmarim on 26/02/2021.
//

#include <kNNEnsembleWPDSM.hpp>

using namespace mltk;

int main(int argc, char *argv[]){
    Data<double> data("bupa.data");
    DSM<double> dsm(data, 7, data.dim() * 0.7, 0);
    auto S = dsm(data);

    for(auto& Si: S){
        for(auto feat: Si){
            std::cout << feat << " ";
        }
        std::cout << std::endl;
    }
}