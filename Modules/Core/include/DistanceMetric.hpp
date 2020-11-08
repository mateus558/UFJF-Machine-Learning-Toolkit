#ifndef DISTANCEMETRIC_HPP_INCLUDED
#define DISTANCEMETRIC_HPP_INCLUDED

#include "Point.hpp"
#include <cmath>

namespace mltk{
    /**
     * \brief Base functor class for the implementation of new distance metrics.
     */
    template < typename T >
    class DistanceMetric {
        public:
            DistanceMetric(){}
            virtual T operator()(const Point<T>& p1, const Point<T>& p2) = 0;    
    };

    /**
     * \brief Functor for the computation of the euclidean distance between two points.
     */
    template < typename T >
    class EuclideanDistance: public DistanceMetric< T > {
        public:
            T operator()(const Point<T>& p1, const Point<T>& p2){
                return sqrt(((p1 - p2) * (p1 - p2)).sum());
            }
    };

    template < typename T >
    class ManhattanDistance: public DistanceMetric< T > {
        public:
            T operator()(const Point<T>& p1, const Point<T>& p2){
                return (p1 - p2).sum([](const T& x){ return abs(x); });
            }
    };
}

#endif