#include "Kernel.hpp"

#include <utility>

namespace mltk{
    using namespace std;

    Kernel::Kernel(int type, double param){
        this->type = type;
        this->param = param;
    }

    Kernel::Kernel(mltk::dMatrix kernel_matrix){
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
        this->computed = false;
    }

    void Kernel::setParam(double _param){
        this->param = _param;
        this->computed = false;
    }

    void Kernel::setKernelMatrix(mltk::dMatrix _K){
        this->K = std::move(_K);
    }

    mltk::dMatrix Kernel::getKernelMatrix(){
        return K;
    }

    mltk::dMatrix* Kernel::getKernelMatrixPointer(){
        return &K;
    }
}

