#include "Point.hpp"
#include <chrono>
#include <iostream>
#include <utility>
#include <fstream>

using namespace std::chrono;

std::pair<double, double> measure(size_t size){
    mltk::Point<double> p(size, 1), q(size, 1), x(size);

    auto t1 = high_resolution_clock::now();
    x = p*q + 2;
    auto t2 = high_resolution_clock::now();
    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < size; ++i){
        x[i] = p[i] * q[i] + 2;
    }
    t2 = high_resolution_clock::now();
    duration<double> time_span2 = duration_cast<duration<double>>(t2-t1);

    return std::make_pair(time_span1.count(), time_span2.count());
}

int main(int argc, char *argv[]){
    std::ofstream measures("measures.dat", std::ios::trunc);

    if(!measures.is_open()){
        std::cerr << "Error opening the output file." << std::endl;
        return 1;
    }

    for(size_t size = 1; size < 1E8; size *= 2){
        auto times = measure(size);
        measures << size << "\t" << times.first << "\t" << times.second << std::endl;
    }
    measures.close();

    return 0;
}