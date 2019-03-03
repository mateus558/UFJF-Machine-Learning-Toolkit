#include "../includes/Kernel.hpp"

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

void Kernel::setType(int type){
    this->type = type;
}

void Kernel::setParam(double param){
    this->param = param;
}

void Kernel::setKernelMatrix(dMatrix K){
    this->K = K;
}

dMatrix Kernel::getKernelMatrix(){
    return K;
}

dMatrix* Kernel::getKernelMatrixPointer(){
    return &K;
}


