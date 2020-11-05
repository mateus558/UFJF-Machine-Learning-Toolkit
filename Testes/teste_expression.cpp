#include "Point.hpp"

int main(int argc, char *argv[]){
    Point<double> x(10, 1);

    x = 3+x+2-x+2.0;

    std::cout << x << std::endl;
}