/*! Kernel manipulation class
   \file Kernel.hpp
   \author Mateus Coutinho Marim
*/
#pragma once
#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>
#include <memory>
#include <cmath>
#include <utility>

#include "Data.hpp"
#include "Utils.hpp"

namespace mltk{
    enum KernelType {INVALID_TYPE = -1, INNER_PRODUCT, POLYNOMIAL, GAUSSIAN, CUSTOM};

    /**
     * \brief Class for the kernel computations.
     */
    template<typename T = double>
    class  Kernel {
        // Attributes
    private :
        template<typename U>
        struct FunctionType
        {
            typedef std::function<double(mltk::Point<U>& a, mltk::Point<U>& b, double param)> Type ;
        } ;

        typename FunctionType<T>::Type func{nullptr};
        bool computed = false;
        /// Kernel type and parameter.
        int type{};
        /// Kernel parameter.
        double param{};
        /// Kernel matrix.
        mltk::dMatrix K;
        /// H matrix.
        mltk::dMatrix H;
        /// H matrix without a dimension.
        mltk::dMatrix HwithoutDim;
    public :
        /**
         * \brief Class constructor.
         */
        Kernel(int type = 0, double param = 0);
        /**
         * \brief getKernelMatrixPointer Returns a reference to the kernel matrix.
         * \return mltk::dMatrix*
         */
        mltk::dMatrix* getKernelMatrixPointer();
        /**
         * \brief Class constructor.
         * \param K Kernel matrix to be set in initialization.
         */
        Kernel(mltk::dMatrix kernel_matrix);
        /**
         * \brief setType Set the kernel type used in the kernel computations.
         * \param type Kernel type.
         */
        void setType(int type);
        /**
         * \brief setParam Set the kernel parameter used in the kernel computations.
         * \param param parameter to be set.
         */
        void setParam(double param);
        /**
         * \brief getType Returns the kernel type used in the kernel computations.
         * \return int.
         */
        int getType();
        /**
         * \brief getParam Returns the kernel parameter used in the kernel computations.
         * \return double
         */
        double getParam();
        /**
         * \brief setKernelMatrix Set a pre computed kernel matrix.
         * \param K Kernel matrix to be set.
         */
        void setKernelMatrix(mltk::dMatrix K);
        /**
         * \brief getKernelMatrix Get the kernel matrix.
         * \return std::vector<std::vector<double> >
         */
        mltk::dMatrix getKernelMatrix();
        void recompute(){ this->computed = false;}
        /**
         * \brief compute Compute the kernel matrix with the given type and parameter.
         * \param samples Data used to compute the kernel matrix.
         */
        void compute(std::shared_ptr<Data< T > > samples,
                     typename FunctionType<T>::Type f = nullptr);
        /**
         * \brief compute Compute the H matrix with the computed kernel matrix and given samples.
         * \param samples Data used to compute the kernel matrix.
         * \return mltk::dMatrix*
         */
        mltk::dMatrix* generateMatrixH(std::shared_ptr<Data< T > > samples);
        /**
         * \brief compute Compute the H matrix without a dimension, with the computed kernel matrix and given samples.
         * \param samples Data used to compute the kernel matrix.
         * \param dim dimension to be ignored.
         * \return mltk::dMatrix*
         */
        mltk::dMatrix* generateMatrixHwithoutDim(std::shared_ptr<Data< T > > samples, int dim);
        /**
         * \brief function Compute the kernel function between two points.
         * \param one first point.
         * \param two second point.
         * \param dim Dimension of the points.
         * \return double
         */
        double function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim,
                        typename FunctionType<T>::Type f = nullptr) const;
        /**
         * \brief function Compute the kernel function between two points without a dimension.
         * \param one first point.
         * \param two second point.
         * \param j Dimension to be ignored.
         * \param dim Dimension of the points.
         * \return double
         */
        double functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim);
        /**
         * \brief norm Computes norm in dual variables.
         * \param data Dataset to compute norm.
         * \return double
         */
        double norm(Data< T > data);
        /**
         * \brief featureSpaceNorm Computes the norm in the feature space (Dual).
         * \param data Dataset to compute norm.
         * \return double
         */
        double featureSpaceNorm(std::shared_ptr<Data< T > > data);

        size_t size() const { return K.size(); }

        double operator()(const Point<T>& a, const Point<T>& b) const{
            assert(a.size() == b.size());
            return function(make_point<T>(a), make_point<T>(b), a.size());
        }

        void operator()(const Data<T>& samples, typename FunctionType<T>::Type f = nullptr){
            compute(make_data<T>(samples), f);
        }

        std::vector<double> operator[](const size_t& idx) const {
            assert(idx < K.size());
            return K[idx];
        }
    };

    template < typename T >
    void Kernel<T>::compute(const std::shared_ptr<Data< T > > samples, typename FunctionType<T>::Type f){
        size_t i, j, size = samples->size(), dim = samples->dim();

        if(computed) return;
        if(this->type == mltk::CUSTOM) this->func = f;
        K.assign(size, std::vector<double>(size, 0.0));

        //Calculating Matrix
        for(i = 0; i < size; ++i){
            for(j = i; j < size; ++j){
                K[i][j] = function((*samples)[i], (*samples)[j], dim, f);
                K[j][i] = K[i][j];
            }
        }
        computed = true;
    }

    template < typename T >
    mltk::dMatrix* Kernel<T>::generateMatrixH(const std::shared_ptr<Data< T > > samples) {
        int i = 0, j = 0;
        size_t size = samples->size(), dim = samples->dim();

        H.resize(size, std::vector<double>(size));

        /* Calculating Matrix */
        for(i = 0; i < size; ++i) {
            for (j = i; j < size; ++j) {
                H[i][j] = function(samples->point(i), samples->point(j), dim) * samples->point(i)->Y() *
                        samples->point(j)->Y();
                H[j][i] = H[i][j];
            }
        }
        return &H;
    }

    template < typename T >
    mltk::dMatrix* Kernel<T>::generateMatrixHwithoutDim(const DataPointer<T> samples, int dim) {
        int i = 0, j = 0;
        size_t size = samples->size();

        for(auto& row: HwithoutDim){
            row.clear();
        }
        HwithoutDim.clear();
        HwithoutDim = mltk::dMatrix(size, std::vector<double>(size, 0.0));

        /* Calculating Matrix */
        for(i = 0; i < size; ++i) {
            for (j = i; j < size; ++j) {
                HwithoutDim[i][j] = functionWithoutDim(samples->point(i), samples->point(j), dim, samples->dim());
                HwithoutDim[j][i] = HwithoutDim[i][j]*samples->point(i)->Y() * samples->point(j)->Y();
            }
        }
    // clog << "\nH matrix without dim generated.\n";
        return &HwithoutDim;
    }

    template < typename T >
    double Kernel<T>::function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim,
                            typename FunctionType<T>::Type f) const{
        int i = 0;
        double t, sum = 0.0;
        std::vector< T > a = one->X(), b = two->X();

        // a.erase(a.end());
        //b.erase(b.end());

        switch(type)
        {
            case mltk::INNER_PRODUCT: //Produto Interno
                for(i = 0; i < dim; ++i)
                    sum += a[i] * b[i];
                break;
            case mltk::POLYNOMIAL: //Polinomial
                for(i = 0; i < dim; ++i)
                    sum += a[i] * b[i];
                //    sum = (param > 1) ? std::pow(sum+1, param) : sum;
                sum = (param > 1) ? std::pow(sum, param) : sum;
                break;

            case mltk::GAUSSIAN: //Gaussiano
                for(i = 0; i < dim; ++i)
                { t = a[i] - b[i]; sum += t * t; }
                sum = std::exp(-1 * sum * param);
                break;

            case mltk::CUSTOM:
                if(f) sum = f(*one, *two, param);
                else sum = this->func(*one, *two, param);
                break;
        }

        /*The '+1' here accounts for the bias term "b" in SVM formulation since
        <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/

        return sum;// + 1.0f;
    }

    template < typename T >
    double Kernel<T>::functionWithoutDim(const PointPointer<T> one, const PointPointer<T> two, int j, int dim) {
        int i = 0;
        double t, sum = 0.0;

        switch(type)
        {
            case mltk::INNER_PRODUCT: //Produto Interno
                for(i = 0; i < dim; ++i)
                    if(i != j)
                        sum += (*one)[i] * (*two)[i];
                break;

            case mltk::POLYNOMIAL: //Polinomial
                for(i = 0; i < dim; ++i)
                    if(i != j)
                        sum += (*one)[i] * (*two)[i];
                sum = (param > 1) ? std::pow(sum+1, param) : sum;
                break;

            case mltk::GAUSSIAN: //Gaussiano
                for(i = 0; i < dim; ++i) {
                    if (i != j) {
                        t = (*one)[i] - (*two)[i];
                        sum += t * t;
                    }
                }
                sum = std::exp(-1 * sum * param);
                break;
            case mltk::CUSTOM:
                sum = this->func(*one, *two, param);
                break;
        }
        /*The '+1' here accounts for the bias term "b" in SVM formulation since
        <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
        return sum;// + 1.0f;
    }

    template < typename T >
    double Kernel<T>::norm(Data< T > data){
        size_t i, j, size = data.size();
        double sum, sum1;
        auto points = data.points();

        sum = sum1 = 0;

        for(i = 0; i < size; ++i){
            for(j = 0; j < size; j++){
                sum1 += points[j]->Alpha() * points[j]->Y() * K[i][j];
                sum += points[i]->Y() * points[i]->Alpha() * sum1;
            }
        }

        return sqrt(sum);
    }

    template < typename T >
    double Kernel<T>::featureSpaceNorm(std::shared_ptr<Data< T > > data) {
        size_t i = 0, j = 0, size = data->size();
        double sum1 = 0.0;
        double sum  = 0.0;

        for(i = 0; i < size; ++i)
        {
            if((*data)[i]->Alpha() > 0)
            {
                sum1 = 0.0;
                for(j = 0; j < size; ++j)
                {
                    if((*data)[j]->Alpha() > 0)
                        sum1 += (*data)[j]->Y() * (*data)[j]->Alpha() * K[j][i];
                }
                sum += (*data)[i]->Alpha() * (*data)[i]->Y() * sum1;
            }
        }
        sum = sqrt(sum);

        return sum;
    }

    template < typename T >
    Kernel<T>::Kernel(int type, double param){
        this->type = type;
        this->param = param;
    }

    template < typename T >
    Kernel<T>::Kernel(mltk::dMatrix kernel_matrix){
        this->K = std::move(kernel_matrix);
    }

    template < typename T >
    int Kernel<T>::getType(){
        return type;
    }

    template < typename T >
    double Kernel<T>::getParam(){
        return param;
    }

    template < typename T >
    void Kernel<T>::setType(int _type){
        this->type = _type;
        this->computed = false;
    }

    template < typename T >
    void Kernel<T>::setParam(double _param){
        this->param = _param;
        this->computed = false;
    }

    template < typename T >
    void Kernel<T>::setKernelMatrix(mltk::dMatrix _K){
        this->K = std::move(_K);
    }

    template < typename T >
    mltk::dMatrix Kernel<T>::getKernelMatrix(){
        return K;
    }

    template < typename T >
    mltk::dMatrix* Kernel<T>::getKernelMatrixPointer(){
        return &K;
    }
}

#endif
