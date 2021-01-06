#include "Point.hpp"
#include <chrono>
#include <iostream>
#include <utility>
#include <fstream>

using namespace std::chrono;

std::pair<double, double> measure(size_t size){
    mltk::Point<double> p(size, 1), q(size, 1), x(size);
    auto t1 = high_resolution_clock::now();
    
    auto sum = (p*q - q + 2).sum();
    //auto sum = mltk::dot(p*q - q + 2, p*q - q + 2);

    auto t2 = high_resolution_clock::now();    
    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);

    t1 = high_resolution_clock::now();
    sum = 0;
    for(size_t i = 0; i < size; ++i){
        sum += (p[i] * q[i] - q[i] + 2) *(p[i] * q[i] - q[i] + 2);
    }
    t2 = high_resolution_clock::now();
//    std::cout << size << " " << sum << std::endl;
//    std::cout << std::endl;
    duration<double> time_span2 = duration_cast<duration<double>>(t2-t1);
    return std::make_pair(time_span1.count(), time_span2.count());
}

int main(int argc, char *argv[]){
    std::ofstream measures("measures.csv", std::ios::trunc);

    if(!measures.is_open()){
        std::cerr << "Error opening the output file." << std::endl;
        return 1;
    }

    measures << "size,Expression templates,Raw loop" << std::endl;
    for(size_t size = 1; size < 1E7; size *= 2){
        auto times = measure(size);
        //measures << size << "," << times.first << "," << times.second << std::endl;
    }
    measures.close();

    return 0;
}