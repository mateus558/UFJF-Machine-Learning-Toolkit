#include "Point.hpp"
#include "DistanceMetric.hpp"

int main(int argc, char *argv[]){
    mltk::Point<double> x(3, 1);
    mltk::Point<double> y(3);

    x = 3*x;
    y = 1;

    std::cout << y << std::endl;

    std::cout << x << std::endl;

    std::cout << mltk::EuclideanDistance<double>()(x, y) << std::endl;
    std::cout << mltk::ManhattanDistance<double>()(x, y) << std::endl;

    return 0;
}