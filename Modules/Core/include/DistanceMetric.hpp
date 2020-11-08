#ifndef DISTANCEMETRIC_HPP_INCLUDED
#define DISTANCEMETRIC_HPP_INCLUDED

#include "Point.hpp"
#include <cmath>

namespace mltk{
    namespace distance{    /**
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
        class Euclidean: public DistanceMetric< T > {
            public:
                T operator()(const Point<T>& p1, const Point<T>& p2){
                    return sqrt(((p1 - p2) * (p1 - p2)).sum());
                }
        };

        template < typename T >
        class Manhattan: public DistanceMetric< T > {
            public:
                T operator()(const Point<T>& p1, const Point<T>& p2){
                    return mltk::abs(p1 - p2).sum();
                }
        };

        template < typename T >
        class Chebyshev: public DistanceMetric< T > {
            public:
                T operator()(const Point<T>& p1, const Point<T>& p2){
                    return mltk::abs(p1-p2).max();
                }
        };
        
    }
}

#endif