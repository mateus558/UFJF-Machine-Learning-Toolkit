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
    y = 1;

    std::cout << y << std::endl;

    std::cout << x << std::endl;

    std::cout << mltk::EuclideanDistance<double>()(x, y) << std::endl;
    std::cout << mltk::ManhattanDistance<double>()(x, y) << std::endl;
    
    std::cout << *k << std::endl;
    return 0;
}