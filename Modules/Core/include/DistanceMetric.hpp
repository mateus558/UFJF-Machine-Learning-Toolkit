#ifndef DISTANCEMETRIC_HPP_INCLUDED
#define DISTANCEMETRIC_HPP_INCLUDED

#include "Point.hpp"
#include <cmath>

template < typename T >
class DistanceMetric {
    public:
        DistanceMetric(){}
        virtual T operator()(const Point<T>& p1, const Point<T>& p2) = 0;    
};


template < typename T >
class EuclideanDistance: public DistanceMetric< T > {
    public:
        T operator()(const Point<T>& p1, const Point<T>& p2){
            return sqrt(((p1 - p2) * (p1 - p2)).sum());
        }
};

#endif