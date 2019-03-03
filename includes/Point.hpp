/*! Point manipulation class
   \file Point.hpp
   \author Mateus Coutinho Marim
*/

#ifndef POINT__HPP
#define POINT__HPP

#include <iostream>
#include <vector>
#include <cmath>

#include "../includes/Utils.hpp"

/**
 * \brief Class of a Point of doubles in a space of n dimensions.
 */

template< typename T > class Point;
template< typename T > std::ostream &operator<<( std::ostream &output, const Point< T > &p );

template< typename T >
class Point {
    // Attributes
public :
    /// Features values.
    std::vector< T > x;
    /// Point classification.
    double y = 0, alpha = 0.0;
    /// Point identification.
    size_t id = 0;

    // Operations
    explicit Point();
    explicit Point(int dim, T val = T(), size_t id = 0);
    /**
     * \brief Computes the dot product with a vector.
     * \param p (???)
     * \return double
     */
    double dot (std::vector< T > p);
    /**
     * \brief Returns the p-norm of the point.
     * \param p (???) p of the norm (euclidean norm is the default).
     * \return double
     */
    double norm (int p = 2);

    /**********************************************
     *  Overloaded operators for the Point class. *
     **********************************************/

    friend std::ostream &operator<< <>( std::ostream &output, const Point< T > &p );

    T operator[](size_t i) const {return x[i];}

    T& operator[](size_t i) {return x[i];}

    bool operator==(const Point< T > &rhs) const;

    bool operator!=(const Point< T > &rhs) const;

    ~Point();
};

template< typename T >
Point< T >::Point(){}

template< typename T >
Point< T >::Point(int dim, T val, size_t id){
    this->id = id;
    x.resize(dim, val);
}

template< typename T >
double Point< T >::dot(std::vector< T > p){
    int i, dim = p.size();
    double dot;

    if(dim != x.size()){
        std::cerr << "The Points must have the same dimension!" << std::endl;
        return -1;
    }

    for(i = 0, dot = 0; i < dim; i++){
        dot += p[i] * x[i];
    }

    return dot;
}

template< typename T >
double Point< T >::norm(int p){
    int i, dim = x.size();
    double norm;

    if(p == NormType::NORM_LINF){
        return maxAbsElement(x);
    }

    for(i = 0, norm = 0; i < dim; i++){
        norm += pow(fabs(x[i]), p);
    }

    return pow(norm, 1.0/p);
}

template< typename T >
std::ostream &operator<<( std::ostream &output, const Point< T > &p ) {
    int i, dim = p.x.size();

    output << p.id << ":[";
    for(i = 0; i < dim; ++i){
        output << p.x[i] << ", ";
    }
    output << p.y << "]";

    return output;
}

template< typename T >
Point< T >::~Point(){

}

template< typename T >
bool Point< T >::operator==(const Point< T > &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           alpha == rhs.alpha &&
           id == rhs.id;
}

template< typename T >
bool Point< T >::operator!=(const Point< T > &rhs) const {
    return !(rhs == *this);
}


#endif
