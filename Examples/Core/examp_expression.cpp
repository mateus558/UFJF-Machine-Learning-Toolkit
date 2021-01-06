#include "Point.hpp"
#include "DistanceMetric.hpp"
#include "Data.hpp"

int main(int argc, char *argv[]){
    mltk::Point<double> x(3, 1);
    mltk::Point<double> y(3);
    mltk::PointPointer<double> k = mltk::make_point<double>(3, 12);
    mltk::DataPointer<double> data = mltk::make_data<double>("iris_mult.csv");

    std::cout << *data << std::endl;

    x = 3*x;
    y = 3.141517;

    std::cout << y << std::endl;
    std::cout << x << std::endl;
    std::cout << *k << std::endl;

    std::cout << mltk::metrics::dist::Euclidean<double>()(x, y) << std::endl;
    std::cout << mltk::metrics::dist::Manhattan<double>()(x, y) << std::endl;
    std::cout << mltk::metrics::dist::Chebyshev<double>()(x, y) << std::endl;
    std::cout << *k << std::endl;
    std::cout << (mltk::pow((*k)*17, 3) % 5).sum() << std::endl;
    
    std::cout << (x-y).norm(mltk::NORM_L2) << std::endl;
    std::cout << (x-y).norm(mltk::NORM_L1) << std::endl;
    std::cout << (x-y).norm(mltk::NORM_LINF) << std::endl;

    return 0;
}