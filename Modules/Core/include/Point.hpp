/*! Point manipulation class
   \file Point.hpp
   \author Mateus Coutinho Marim
*/

#ifndef POINT__HPP
#define POINT__HPP

#include <cstddef>
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>

#include "ExprOps.hpp"
#include "ExprScalar.hpp"
#include "Utils.hpp"

template <typename T, typename Rep> class Point;
template< typename T, typename Rep> std::ostream &operator<<( std::ostream &output, const Point<T, Rep> &p );

template <typename T, typename Rep = std::vector<T>>
class Point {
    private:
        /// Features values.
        Rep x; // (access to) the data of the array
        /// Point classification.
        double y = 0, alpha = 0.0;
        /// Point identification.
        size_t id = 0;

    public:
        Point(){}
        // create array with initial size
        explicit Point(std::size_t s): x(s) {}
        // create array with initial size, default value and optional id
        Point(std::size_t s, const T &value, const std::size_t &id = 0): x(s, value), id(id) {}
        // create array from possible representation
        Point(Rep const& rb): x(rb) {}
        /**
         * \brief Computes the dot product with a vector.
         * \param p (???)
         * \return double
         */
        T dot (const std::vector< T > &p){
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
        /**
         * \brief Returns the p-norm of the point.
         * \param p (???) p of the norm (euclidean norm is the default).
         * \return double
         */
        double norm (int p = 2){
            int i, dim = x.size();
            double norm;

            if(p == NormType::NORM_LINF){
                return Utils::maxAbsElement(x);
            }

            for(i = 0, norm = 0; i < dim; i++){
                norm += pow(fabs(x[i]), p);
            }

            return pow(norm, 1.0/p);
        }

        T sum(){ 
            T _sum = T();
            for(std::size_t i = 0; i < size(); i++){
                _sum += x[i];
            }
            return _sum;
        }

        // assignment operator from same type
        Point& operator=(Point const& b) {
            assert(size() == b.size());
            for(std::size_t idx = 0; idx < b.size(); ++idx) {
                x[idx] = b[idx];
            }
            return *this;
        }

        // assignment operator for arrays of different types
        template <typename T2, typename Rep2 >
        Point& operator=(Point<T2, Rep2> const& b) {
            assert(size() == b.size());
            for(std::size_t idx = 0; idx < b.size(); ++idx){
                x[idx] = b[idx];
            }
            return *this;
        }

        // size is size of represented data
        std::size_t size() const {
            return x.size();
        }

        // index operator for constants and variables
        decltype(auto) operator[] (std::size_t idx) const {
            assert(idx < size());
            return x[idx];
        }

        T& operator[](std::size_t idx) {
            assert(idx < size());
            return x[idx];
        }

        // return what the array currently represents
        Rep const& rep() const{
            return x;
        }

        Rep& rep(){
            return x;
        }

        double const& y() const{
            return y;
        }

        double& y(){
            return y;
        }

        void setY(double const& y){
            this->y = y;
        }

        double const& alpha() const{
            return y;
        }

        double& alpha(){
            return y;
        }

        void setAlpha(double const& alpha){
            this->alpha = alpha;
        }

        double const& id() const{
            return id;
        }

        double& id(){
            return id;
        }

        void setID(double const& id){
            this->id = id;
        }

        friend std::ostream &operator<< <T, Rep>( std::ostream &output, const Point< T, Rep> &p );
        bool operator== (Point const& rhs) const;
        bool operator!= (Point const& rhs) const;
};

/*************************************************
 *  Overloaded operators for the Point template. *
 *************************************************/

template <typename T, typename R>
std::ostream &operator<<( std::ostream &output, const Point<T, R> &p ) {
    int i, dim = p.x.size();

    output << p.id << ":[";
    for(i = 0; i < dim; ++i){
        output << p.x[i] << ", ";
    }
    output << p.y << "]";

    return output;
}

// equality of two points
template <typename T, typename R>
bool Point<T, R>::operator== (Point<T, R> const& rhs) const{
    return x == rhs.x &&
           y == rhs.y &&
           alpha == rhs.alpha &&
           id == rhs.id;
}

// difference of two points
template< typename T, typename R >
bool Point< T, R >::operator!=(const Point<T, R> &rhs) const {
    return !(rhs == *this);
}

// adition of two points
template <typename T, typename R1, typename R2>
Point<T, A_Add<T, R1, R2>>
operator+ (Point<T, R1> const& a, Point<T, R2> const& b){
    return Point<T, A_Add<T, R1, R2>>(A_Add<T, R1, R2>(a.rep(), b.rep()));
}

// adition of scalar and point
template <typename T, typename R1, typename R2>
Point<T, A_Add<T, A_Scalar<T>, R2>>
operator+ (T const& s, Point<T, R2> const& b){
    return Point<T, A_Add<T, A_Scalar<T>, R2>>(A_Add<T, A_Scalar<T>, R2>(A_Scalar<T>(s), b.rep()));
}

// adition of point and scalar
template <typename T, typename R1, typename R2>
Point<T, A_Add<T, R1, A_Scalar<T>>>
operator+ (Point<T, R1> const& a, T const& s){
    return Point<T, A_Add<T, R1, A_Scalar<T>>>(A_Add<T, R1, A_Scalar<T>>(a.rep(), A_Scalar<T>(s)));
}

// subtraction of two points
template <typename T, typename R1, typename R2>
Point<T, A_Sub<T, R1, R2>>
operator- (Point<T, R1> const& a, Point<T, R2> const& b){
    return Point<T, A_Sub<T, R1, R2>>(A_Sub<T, R1, R2>(a.rep(), b.rep()));
}

// subtraction of scalar and array
template <typename T, typename R1, typename R2>
Point<T, A_Sub<T, A_Scalar<T>, R2>>
operator- (T const& s, Point<T, R2> const& b){
    return Point<T, A_Sub<T, A_Scalar<T>, R2>>(A_Sub<T, A_Scalar<T>, R2>(A_Scalar<T>(s), b.rep()));
}

// subtraction of point and scalar
template <typename T, typename R1, typename R2>
Point<T, A_Sub<T, R1, A_Scalar<T>>>
operator- (Point<T, R1> const& a, T const& s){
    return Point<T, A_Sub<T, R1, A_Scalar<T>>>(A_Sub<T, R1, A_Scalar<T>>(a.rep(), A_Scalar<T>(s)));
}

// multiplication of two points
template <typename T, typename R1, typename R2>
Point<T, A_Mult<T, R1, R2>>
operator* (Point<T, R1> const& a, Point<T, R2> const& b){
    return Point<T, A_Mult<T, R1, R2>>(A_Mult<T, R1, R2>(a.rep(), b.rep()));
}

// multiplication of scalar and point
template<typename T, typename R2>
Point<T, A_Mult<T,A_Scalar<T>,R2>>
operator* (T const& s, Point<T,R2> const& b) {
    return Point<T,A_Mult<T,A_Scalar<T>,R2>>(A_Mult<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.rep()));
}

// multiplication of array and scalar
template <typename T, typename R1>
Point<T, A_Mult<T, R1, A_Scalar<T>>>
operator* (Point<T, R1> const& a, T const& s){
    return Point<T, A_Mult<T, R1, A_Scalar<T>>>(A_Mult<T, R1, A_Scalar<T>>(a.rep(), A_Scalar<T>(s)));
}

// division of two points
template <typename T, typename R1, typename R2>
Point<T, A_Div<T, R1, R2>>
operator/ (Point<T, R1> const & a, Point<T, R2> const& b){
    return Point<T, A_Div<T, R1, R2>>(A_Div<T, R1, R2>(a.rep(), b.rep()));
}

// division of point and scalar
template <typename T, typename R1>
Point<T, A_Div<T, R1, A_Scalar<T>>>
operator/ (Point<T, R1> const& a, T const& s){
    return Point<T, A_Div<T, R1, A_Scalar<T>>>(A_Div<T, R1, A_Scalar<T>>(a.rep(), A_Scalar<T>(s)));
}

// division of scalar and point
template<typename T, typename R2>
Point<T, A_Div<T,A_Scalar<T>,R2>>
operator/ (T const& s, Point<T,R2> const& b) {
    return Point<T,A_Div<T,A_Scalar<T>,R2>>(A_Div<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.rep()));
}

#endif