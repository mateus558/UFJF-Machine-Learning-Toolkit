#include "../includes/Kernel.hpp"

#include <utility>

using namespace std;

Kernel::Kernel(int type, double param){
    this->type = type;
    this->param = param;
}

Kernel::Kernel(dMatrix kernel_matrix){
    this->K = std::move(kernel_matrix);
}

int Kernel::getType(){
    return type;
}

double Kernel::getParam(){
    return param;
}

void Kernel::setType(int _type){
    this->type = _type;
}

void Kernel::setParam(double _param){
    this->param = _param;
}

void Kernel::setKernelMatrix(dMatrix _K){
    this->K = std::move(_K);
}

dMatrix Kernel::getKernelMatrix(){
    return K;
}

dMatrix* Kernel::getKernelMatrixPointer(){
    return &K;
}


