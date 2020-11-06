#include "Point.hpp"

int main(int argc, char *argv[]){
    Point<double> x(100, 1);
    Point<int> y(100, 1);

    x = 3*x;

    x += 1;
    y *= 2;
    x /= 3;
    std::cout << y << std::endl;

    std::cout << x << std::endl;

    //x[y] = y;
}