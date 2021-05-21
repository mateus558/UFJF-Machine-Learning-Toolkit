/*! Point manipulation class
   \file Point.hpp
   \author Mateus Coutinho Marim
*/

#pragma once
#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLPoint __declspec(dllexport)
    #define DLLPointDecl __cdecl
  #else
    #define DLLPoint __declspec(dllimport)
    #define DLLPointDecl
  #endif
#else
  #ifdef BUILDING_CORE
    #define DLLPoint __attribute__ ((visibility ("default")))
    #define DLLPointDecl
  #else
      #define DLLPoint
      #define DLLPointDecl
  #endif
#endif

#include <cstddef>
#include <cassert>
#include <vector>
#include <iostream>
#include <functional>
#include <cmath>
#include <memory>
#include <algorithm>
#include <initializer_list>

#include "ExprOps.hpp"
#include "ExprScalar.hpp"
#include "Utils.hpp"
#include "Random.hpp"

/**
 * \brief UFJF-MLTK main namespace for core functionalities.
 */
namespace mltk {    
    template <typename T, typename Rep > class Point;
    template <typename T, typename Rep> std::ostream &operator<<( std::ostream &output, const Point<T, Rep> &p );
    template <class T, typename Rep = std::vector<T> > using PointPointer = std::shared_ptr<mltk::Point<T, Rep> >;
    template <class T, typename Rep = std::vector<T> > using PointIterator = typename Rep::iterator ;

    /**
     * \brief Wrapper for the point data.
     */
    template <typename T = double, typename Rep = std::vector<T> >
    class DLLPoint Point {
        private:
            /// Features values.
            Rep x; // (access to) the data of the array
            /// Point classification.
            double y = -std::numeric_limits<double>::max(), alpha = 0.0;
            /// Point identification.
            size_t id = 0;

        public:
            auto begin() const {
                return x.cbegin();
            }
            auto end() const{
                return x.cend();
            }
            auto begin() {
                return x.begin();
            }
            auto end() {
                return x.end();
            }
            /**
             * \brief Empty constructor.
             **/
            Point(){}

            Point(Point<T> const &p){
                this->x = p.X();
                this->y = p.Y();
                this->id = p.Id();
                this->alpha = p.Alpha();
            }

            template<typename R1>
            Point(Point<T, R1> const &p){
                if(size() == 0){
                    x.resize(p.size());
                }
                for(size_t i = 0; i < p.size(); i++){
                    this->x[i] = p[i];
                }
                this->y = p.Y();
                this->id = p.Id();
                this->alpha = p.Alpha();
            }

            template<typename T2, typename R1>
            Point(Point<T2, R1> const &p){
                if(size() == 0){
                    x.resize(p.size());
                }
                for(size_t i = 0; i < p.size(); i++){
                    this->x[i] = p[i];
                }
                this->y = p.Y();
                this->id = p.Id();
                this->alpha = p.Alpha();
            }
            /**
             * \brief Construct a point with initial dimension.
             * \param s Dimension of the point.
             **/
            explicit Point(std::size_t s): x(s) {}
            /**
             * \brief Construct a point with initial dimension, default value and id.
             * \param s Dimension of the point.
             * \param value Initial value of the dimensions.
             * \param id Id of the point
             **/
            Point(std::size_t s, const T &value, const std::size_t &id = 0): x(s, value), id(id) {}
            /**
             * \brief Construct a point with a custom internal representation.
             * \param rb Point internal representation (std::vector by default).
             **/
            Point(Rep const& rb): x(rb) {}

            Point(std::initializer_list<T> init): x(init) {}

            /*********************************************
             *               Getters                     *
             *********************************************/
            
            /**
             * \brief Returns the dimension of the point.
             * \return std::size_t
             **/
            std::size_t size() const {
                return x.size();
            }
            /**
             * \brief Returns the attributes representation of the point (std::vector by default).
             **/
            Rep const& X() const{
                return x;
            }
            /**
             * \brief Returns a reference to the attributes representation of the point (std::vector by default).
             **/
            Rep& X(){
                return x;
            }
            /**
             * \brief Returns the class or value of the point.
             * \return double
             **/
            double const& Y() const{
                return y;
            }
            /**
             * \brief Returns a reference to the class or value of the point.
             * \return double
             **/
            double& Y(){
                return y;
            }
            /**
             * \brief Return the alpha value of the point.
             * \return double
             **/
            double const& Alpha() const{
                return alpha;
            }
            /**
             * \brief Returns a reference to the alpha value of the point.
             * \return double
             **/
            double& Alpha(){
                return alpha;
            }
             /**
             * \brief Returns the id of the point.
             * \return size_t
             **/
            size_t const& Id() const{
                return id;
            }
            /**
            * \brief Returns a reference to the id of the point.
            * \return size_t
            **/
            size_t& Id(){
                return id;
            }

            /*********************************************
             *               Setters                     *
             *********************************************/
            
            /**
             * \brief Set the class or value of the point.
             * \param y The class/value of the point.
             **/
            void setY(double const& _y){
                this->y = _y;
            }
            /**
             * \brief Set alpha value of the point.
             * \param alpha alpha value.
             **/
            void setAlpha(double const& _alpha){
                this->alpha = _alpha;
            }
            /**
             * \brief Set the id of the point.
             * \param _id new id of the point.
             **/
            void setID(size_t const& _id){
                this->id = _id;
            }

            Point<T> selectFeatures(std::vector<size_t> feats) const {
                Point<T> p(feats.size());
                std::sort(feats.begin(), feats.end());

                size_t i = 0;
                for(auto const& feat: feats){
                    assert(feat < x.size());
                    p[i] = x[feat];
                    i++;
                }
                p.Y() = y;
                p.Id() = id;
                p.Alpha() = alpha;
                return p;
            }

            /*********************************************
             *               Access operators            *
             *********************************************/
            
            // index operator for constants and variables
            decltype(auto) operator[] (const size_t &idx) const {
                assert(idx < size());
                return x[idx];
            }

            T& operator[](const size_t &idx) {
                assert(idx < size());
                return x[idx];
            }
            
            template<typename T2, typename R2>
            Point<T, A_Subscript<T, Rep, R2>> operator[](Point<T2, R2> const& b) {
                return Point<T, A_Subscript<T, Rep, R2>> (A_Subscript<T, Rep, R2>((*this).X(), b.X()));
            }

            template <typename... Types>
            void resize(Types... args){
                this->x.resize(args...);
            }
            template <typename... Types>
            void assign(Types... args){
                this->x.assign(args...);
            }
            /**
             * \brief Returns the p-norm of the point.
             * \param p p of the norm (euclidean norm is the default).
             * \return double
             */
            double norm (int p = NormType::NORM_L2){
                if(p == NormType::NORM_LINF){
                    return max(abs(*this));
                }
                return std::pow(pow(abs(*this), p).sum(), 1.0/p);
            }
            /**
             * \brief Compute the sum of the components of the point.
             * \return The sum of the components of the point.
             **/
            T sum(const std::function <T (T)>& f = [](T const& t) { return t;}) const{
                T _sum = T();

                for(std::size_t i = 0; i < size(); i++){
                    _sum += f(x[i]);
                }
                
                return _sum;
            }

            /*********************************************
             *                Other operators            *
             *********************************************/

            // assignment operator from same type
            Point& operator=(Point const& b) {
                if(size() == 0){
                    x.resize(b.size());
                }

                for(std::size_t idx = 0; idx < b.size(); ++idx) {
                    x[idx] = b[idx];
                }
                
                return *this;
            }

            // assignment operator for arrays of different types
            template <typename T2, typename Rep2 >
            Point& operator=(Point<T2, Rep2> const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] = b[idx];
                }

                return *this;
            }

            Point& operator=(std::vector<T> const& b) {
                if(size() == 0){
                    x.resize(b.size());
                }

                for(std::size_t idx = 0; idx < b.size(); idx++) {
                    x[idx] = b[idx];
                }
                
                return *this;
            }

            template< typename T2 >
            Point& operator=(std::vector<T2> const& b) {
                if(size() == 0){
                    x.resize(b.size());
                }

                for(std::size_t idx = 0; idx < b.size(); ++idx) {
                    x[idx] = b[idx];
                }

                return *this;
            }

            Point& operator=(T const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] = b;
                }
                return *this;
            }

            template <typename Y>
            Point& operator=(Y const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] = b;
                }
                return *this;
            }

            // plus assignment operator for arrays of same types
            Point& operator+=(Point const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] += b[idx];
                }
                return *this;
            }

            Point& operator+=(T const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] += b;
                }
                return *this;
            }

            template <typename Y>
            Point& operator+=(Y const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] += b;
                }
                return *this;
            }

            // plus assignment operator for arrays of different types
            template <typename T2, typename Rep2 >
            Point& operator+=(Point<T2, Rep2> const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] += b[idx];
                }
                return *this;
            }

            // minus assignment operator for arrays of same types
            Point& operator-=(Point const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] -= b[idx];
                }
                return *this;
            }

            // minus assignment operator for arrays of different types
            template <typename T2, typename Rep2 >
            Point& operator-=(Point<T2, Rep2> const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] -= b[idx];
                }
                return *this;
            }

            Point& operator-=(T const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] -= b;
                }
                return *this;
            }

            template <typename Y>
            Point& operator-=(Y const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] -= b;
                }
                return *this;
            }

            // minus assignment operator for arrays of same types
            Point& operator*=(Point const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] *= b[idx];
                }
                return *this;
            }

            // minus assignment operator for arrays of different types
            template <typename T2, typename Rep2 >
            Point& operator*=(Point<T2, Rep2> const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] *= b[idx];
                }
                return *this;
            }

            Point& operator*=(T const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] *= b;
                }
                return *this;
            }

            template <typename Y>
            Point& operator*=(Y const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] *= b;
                }
                return *this;
            }

            // minus assignment operator for arrays of same types
            Point& operator/=(Point const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] /= b[idx];
                }
                return *this;
            }

            // minus assignment operator for arrays of different types
            template <typename T2, typename Rep2 >
            Point& operator/=(Point<T2, Rep2> const& b) {
                assert(size() == b.size());
                for(std::size_t idx = 0; idx < b.size(); ++idx){
                    x[idx] /= b[idx];
                }
                return *this;
            }

            Point& operator/=(T const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] /= b;
                }
                return *this;
            }

            template <typename Y>
            Point& operator/=(Y const& b) {
                for(std::size_t idx = 0; idx < size(); ++idx){
                    x[idx] /= b;
                }
                return *this;
            }
            
            friend std::ostream &operator<< <T, Rep>( std::ostream &output, const Point< T, Rep> &p );
            bool operator== (Point const& rhs) const;
            bool operator!= (Point const& rhs) const;
    };

    /*************************************************
     *  Overloaded operators for the Point template. *
     *************************************************/

    template < typename T, typename R = std::vector<T>, typename... Types>
    PointPointer<T, R> make_point(Types... args){
        return std::make_shared< Point< T, R > >(args...);
    }

    /*********************************************
     *               Point functions             *
     *********************************************/

    /**
     * \brief Computes the dot product with a vector.
     * \param p First point from dot product 
     * \param p1 Second point from dot product
     * \return double
     */
    template < typename T, typename R >
    T dot (const Point<T, R> &p, const Point<T, R> &p1){
        assert(p.size() == p1.size());
        size_t dim = p.size();
        T result = 0;
        for(size_t i = 0; i < dim; i++){
            result += p[i] * p1[i];
        }

        return result;
    }

     /**
     * \brief Returns the max value of the point.
     * \return T
     */
    template < typename T, typename R >
    T max(const Point<T, R> &p){
        T _max = std::numeric_limits<T>::min();
        for(size_t i = 0; i < p.size(); i++){
            if(p[i] > _max) _max = p[i];
        }
        return _max;
    }

    /**
     * \brief Returns the min value of the point.
     * \return T
     */
    template < typename T, typename R >
    T min(const Point<T, R> &p){
        T _min = std::numeric_limits<T>::max();
        for(size_t i = 0; i < p.size(); i++){
            if(p[i] < _min) _min = p[i];
        }
        return _min;
    }

    template < typename T = double, typename R = std::vector< T > >
    Point<T, R> random_init(Point<T, R> &p, const size_t &size, const size_t &seed){
        mltk::random::init(seed);

        p.X().resize(size);
        for(size_t i = 0; i < p.size(); i++){
            p[i] = mltk::random::floatInRange<T>(0.0, 1.0);
        }
        p.Y() = 0;
        return p;
    }

    template < typename T = double, typename R = std::vector< T > >
    Point<T, R> random_init(const size_t &size, const size_t &seed=0){
        Point<T, R> p(size);
        mltk::random::init(seed);

        for(size_t i = 0; i < p.size(); i++){
            p[i] = mltk::random::floatInRange(0.0, 1.0);
        }
        p.Y() = 0;
        return p;
    }

    template < typename T = double, typename distribution = std::uniform_real_distribution<T> >
    Point<T> random_init(T val1, T val2, const size_t &size, const size_t &seed=0){
        Point<T> p(size);
        mltk::random::init(seed);

        for(size_t i = 0; i < p.size(); i++){
            p[i] = mltk::random::floatInRange<T,T, distribution>(val1, val2);
        }
        p.Y() = 0;
        return p;
    }

    template < typename T = double, typename R = std::vector< T > >
    Point<T, R> random_init(Point<T, R> &p, const size_t &seed=0){
        mltk::random::init(seed);

        for(size_t i = 0; i < p.size(); i++){
            p[i] = mltk::random::floatInRange(0.0, 1.0);
        }
        p.Y() = 0;
        return p;
    }

    template < typename T, typename R>
    Point<T, F_Abs<T, R> > abs(const Point<T, R>& p){        
        return Point<T, F_Abs<T, R > >(F_Abs<T, R>(p.X()));
    }

    template < typename T, typename R>
    Point<T, F_Cos<T, R> > cos(const Point<T, R>& p){
        return Point<T, F_Cos<T, R > >(F_Cos<T, R>(p.X()));
    }

    template < typename T, typename R>
    Point<T, F_Sin<T, R> > sin(const Point<T, R>& p){
        return Point<T, F_Sin<T, R > >(F_Sin<T, R>(p.X()));
    }

    template < typename T, typename R>
    Point<T, F_Exp<T, R> > DLLPointDecl exp(const Point<T, R>& p){
        return Point<T, F_Exp<T, R > >(F_Exp<T, R>(p.X()));
    }

    template < typename T, typename R>
    Point<T, F_Log<T, R> > DLLPointDecl log(const Point<T, R>& p){
        return Point<T, F_Log<T, R > >(F_Log<T, R>(p.X()));
    }

    template < typename T, typename R>
    Point<T, F_Pow<T, T, R> > DLLPointDecl pow(const Point<T, R>& p, const T &power){        
        return Point<T, F_Pow<T, T, R > >(F_Pow<T, T, R>(p.X(), power));
    }

    template < typename T, typename P, typename R>
    Point<T, F_Pow<T, P, R> > DLLPointDecl pow(const Point<T, R>& p, const P &power){        
        return Point<T, F_Pow<T, P, R > >(F_Pow<T, P, R>(p.X(), power));
    }
    /**
     * \brief normalize Normalize a vector using a Lp-norm.
     * \param q Norm to be utilized.
     * \param p Vector to be normalized.
     */
    template < typename T>
    Point<T> normalize (Point<T> p, const double q){
        double norm = std::pow(mltk::pow(mltk::abs(p), q).sum(), 1.0/q);
        p /= norm;
        return p;
    }

    template < typename T>
    Point<T> normalize (std::vector<T> &p, const double q){
        Point<T> result = p;
        double norm = std::pow(mltk::pow(mltk::abs(result), q).sum(), 1.0/q);
        result /= norm;
        p = result.X();
        return result;
    }

    template < typename T = double>
    Point<T> linspace(double lower, double upper, size_t N){
        double h = (upper - lower) / static_cast<double>(N-1);
        std::vector<T> xs(N);
        typename std::vector<T>::iterator x;
        T val;
        for (x = xs.begin(), val = lower; x != xs.end(); ++x, val += h) {
            *x = val;
        }
        return xs;
    }

    template <typename T, typename R>
    std::ostream &operator<<( std::ostream &output, const Point<T, R> &p ) {
        int i, dim = p.x.size();

        if(p.id > 0)
            output << p.id << ":[";
        else
            output << "[";
        
        for(i = 0; i < dim-1; ++i){
            output << p.x[i] << ", ";
        }

        if(p.y > -std::numeric_limits<double>::max())
            output <<  p.x[i] << ", " << p.y << "]";
        else
            output <<  p.x[i] << "]";

        return output;
    }

    // equality of two points
    template <typename T, typename R>
    bool Point<T, R>::operator== (Point<T, R> const& rhs) const{
        return x == rhs.x;
    }

    // difference of two points
    template< typename T, typename R >
    bool Point< T, R >::operator!=(const Point<T, R> &rhs) const {
        return !(rhs == *this);
    }

    // module of point and scalar
    template<typename T, typename R1>
    Point<T, A_Mod<T, R1, A_Scalar<T> > > operator%(const Point<T, R1>& p, const T& mod){
        return Point<T, A_Mod<T, R1, A_Scalar<T>> >(A_Mod<T, R1, A_Scalar<T>>(p.X(), A_Scalar<T>(mod)));
    } 

    // module of point and scalar
    template<typename T, typename Y, typename R1>
    Point<T, A_Mod<T, R1, A_Scalar<Y> > > operator%(const Point<T, R1>& p, const Y& mod){
        return Point<T, A_Mod<T, R1, A_Scalar<Y>> >(A_Mod<T, R1, A_Scalar<Y>>(p.X(), A_Scalar<Y>(mod)));
    }

    // adition of two points
    template <typename T, typename R1, typename R2>
    Point<T, A_Add<T, R1, R2> > operator+ (Point<T, R1> const& a, Point<T, R2> const& b){
        return Point<T, A_Add<T, R1, R2>>(A_Add<T, R1, R2>(a.X(), b.X()));
    }

    // addition of scalar and point
    template<typename T, typename R2>
    Point<T, A_Add<T,A_Scalar<T>,R2> > operator+ (T const& s, Point<T,R2> const& b) {
        return Point<T,A_Add<T,A_Scalar<T>,R2>>(A_Add<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.X()));
    }

    // addition of scalar and point
    template<typename T, typename T2, typename R2>
    Point<T, A_Add<T,A_Scalar<T2>,R2> > operator+ (T2 const& s, Point<T,R2> const& b) {
        return Point<T,A_Add<T,A_Scalar<T2>,R2>>(A_Add<T,A_Scalar<T2>,R2>(A_Scalar<T2>(s), b.X()));
    }

    // addition of point and scalar
    template <typename T, typename R1>
    Point<T, A_Add<T, R1, A_Scalar<T> > > operator+ (Point<T, R1> const& a, T const& s){
        return Point<T, A_Add<T, R1, A_Scalar<T>>>(A_Add<T, R1, A_Scalar<T>>(a.X(), A_Scalar<T>(s)));
    }

    // addition of point and scalar
    template <typename T, typename T2, typename R1>
    Point<T, A_Add<T, R1, A_Scalar<T2> > > operator+ (Point<T, R1> const& a, T2 const& s){
        return Point<T, A_Add<T, R1, A_Scalar<T2>>>(A_Add<T, R1, A_Scalar<T2>>(a.X(), A_Scalar<T2>(s)));
    }

    // subtraction of two points
    template <typename T, typename R1, typename R2>
    Point<T, A_Sub<T, R1, R2> > operator- (Point<T, R1> const& a, Point<T, R2> const& b){
        return Point<T, A_Sub<T, R1, R2>>(A_Sub<T, R1, R2>(a.X(), b.X()));
    }

    // subtraction of scalar and point
    template<typename T, typename R2>
    Point<T, A_Sub<T,A_Scalar<T>,R2> > operator- (T const& s, Point<T,R2> const& b) {
        return Point<T,A_Sub<T,A_Scalar<T>,R2>>(A_Sub<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.X()));
    }

    // subtraction of scalar and point
    template<typename T, typename R2>
    Point<T, A_Sub<T,A_Scalar<int>,R2> > operator- (int const& s, Point<T,R2> const& b) {
        return Point<T,A_Sub<T,A_Scalar<int>,R2>>(A_Sub<T,A_Scalar<int>,R2>(A_Scalar<int>(s), b.X()));
    }

    // subtraction of point and scalar
    template <typename T, typename R1>
    Point<T, A_Sub<T, R1, A_Scalar<T> > > operator- (Point<T, R1> const& a, T const& s){
        return Point<T, A_Sub<T, R1, A_Scalar<T>>>(A_Sub<T, R1, A_Scalar<T>>(a.X(), A_Scalar<T>(s)));
    }

    // subtraction of point and scalar
    template <typename T, typename R1>
    Point<T, A_Sub<T, R1, A_Scalar<int> > > operator- (Point<T, R1> const& a, int const& s){
        return Point<T, A_Sub<T, R1, A_Scalar<int>>>(A_Sub<T, R1, A_Scalar<int>>(a.X(), A_Scalar<int>(s)));
    }

    // multiplication of two points
    template <typename T, typename R1, typename R2>
    Point<T, A_Mult<T, R1, R2> > operator* (Point<T, R1> const& a, Point<T, R2> const& b){
        return Point<T, A_Mult<T, R1, R2>>(A_Mult<T, R1, R2>(a.X(), b.X()));
    }

    // multiplication of scalar and point
    template<typename T, typename R2>
    Point<T, A_Mult<T,A_Scalar<T>,R2> > operator* (T const& s, Point<T,R2> const& b) {
        return Point<T,A_Mult<T,A_Scalar<T>,R2>>(A_Mult<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.X()));
    }

    // multiplication of scalar and point
//    template<typename T, typename T2, typename R2>
//    Point<T, A_Mult<T,A_Scalar<T2>,R2> > operator* (T2 const& s, Point<T,R2> const& b) {
//        return Point<T,A_Mult<T,A_Scalar<T2>,R2>>(A_Mult<T,A_Scalar<T2>,R2>(A_Scalar<T2>(s), b.X()));
//    }
    // multiplication of scalar and point
    template <typename T, typename T2, typename R1>
    Point<T, A_Mult<T, A_Scalar<T2>, R1> > operator* (T2 const& s, Point<T, R1> const& b){
        return Point<T, A_Mult<T, A_Scalar<T2>, R1>>(A_Mult<T, A_Scalar<T2>, R1>(A_Scalar<T2>(s), b.X()));
    }

    // multiplication of point and scalar
    template <typename T, typename R1>
    Point<T, A_Mult<T, R1, A_Scalar<T> > > operator* (Point<T, R1> const& a, T const& s){
        return Point<T, A_Mult<T, R1, A_Scalar<T>>>(A_Mult<T, R1, A_Scalar<T>>(a.X(), A_Scalar<T>(s)));
    }

    // multiplication of point and scalar
    template <typename T, typename T2, typename R1>
    Point<T, A_Mult<T, R1, A_Scalar<T2> > > operator* (Point<T, R1> const& a, T2 const& s){
        return Point<T, A_Mult<T, R1, A_Scalar<T2>>>(A_Mult<T, R1, A_Scalar<T2>>(a.X(), A_Scalar<T2>(s)));
    }

    // division of two points
    template <typename T, typename R1, typename R2>
    Point<T, A_Div<T, R1, R2> > operator/ (Point<T, R1> const & a, Point<T, R2> const& b){
        return Point<T, A_Div<T, R1, R2>>(A_Div<T, R1, R2>(a.X(), b.X()));
    }

    // division of point and scalar
    template <typename T, typename R1>
    Point<T, A_Div<T, R1, A_Scalar<T> > > operator/ (Point<T, R1> const& a, T const& s){
        return Point<T, A_Div<T, R1, A_Scalar<T>>>(A_Div<T, R1, A_Scalar<T>>(a.X(), A_Scalar<T>(s)));
    }

    // division of point and scalar
    template <typename T, typename R1>
    Point<T, A_Div<T, R1, A_Scalar<int> > > operator/ (Point<T, R1> const& a, int const& s){
        return Point<T, A_Div<T, R1, A_Scalar<int>>>(A_Div<T, R1, A_Scalar<int>>(a.X(), A_Scalar<int>(s)));
    }

    // division of scalar and point
    template<typename T, typename R2>
    Point<T, A_Div<T,A_Scalar<T>,R2> > operator/ (T const& s, Point<T,R2> const& b) {
        return Point<T,A_Div<T,A_Scalar<T>,R2>>(A_Div<T,A_Scalar<T>,R2>(A_Scalar<T>(s), b.X()));
    }

    // division of scalar and point
    template<typename T, typename R2>
    Point<T, A_Div<T,A_Scalar<int>,R2> > operator/ (int const& s, Point<T,R2> const& b) {
        return Point<T,A_Div<T,A_Scalar<int>,R2>>(A_Div<T,A_Scalar<int>,R2>(A_Scalar<int>(s), b.X()));
    }

}

#endif