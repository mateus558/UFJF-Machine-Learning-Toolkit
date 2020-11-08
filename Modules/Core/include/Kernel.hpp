/*! Kernel manipulation class
   \file Kernel.hpp
   \author Mateus Coutinho Marim
*/

#ifndef KERNEL__HPP
#define KERNEL__HPP
#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <utility>

#include "Data.hpp"
#include "Utils.hpp"

namespace mltk{
    enum KernelType {INVALID_TYPE = -1, INNER_PRODUCT, POLYNOMIAL, GAUSSIAN};

    /**
     * \brief Class for the kernel computations.
     */
    class Kernel {
        // Attributes
    private :
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
        template < typename T >
        void compute(std::shared_ptr<Data< T > > samples);
        /**
         * \brief compute Compute the H matrix with the computed kernel matrix and given samples.
         * \param samples Data used to compute the kernel matrix.
         * \return mltk::dMatrix*
         */
        template < typename T >
        mltk::dMatrix* generateMatrixH(std::shared_ptr<Data< T > > samples);
        /**
         * \brief compute Compute the H matrix without a dimension, with the computed kernel matrix and given samples.
         * \param samples Data used to compute the kernel matrix.
         * \param dim dimension to be ignored.
         * \return mltk::dMatrix*
         */
        template < typename T >
        mltk::dMatrix* generateMatrixHwithoutDim(std::shared_ptr<Data< T > > samples, int dim);
        /**
         * \brief function Compute the kernel function between two points.
         * \param one first point.
         * \param two second point.
         * \param dim Dimension of the points.
         * \return double
         */
        template < typename T >
        double function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim);
        /**
         * \brief function Compute the kernel function between two points without a dimension.
         * \param one first point.
         * \param two second point.
         * \param j Dimension to be ignored.
         * \param dim Dimension of the points.
         * \return double
         */
        template < typename T >
        double functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim);
        /**
         * \brief norm Computes norm in dual variables.
         * \param data Dataset to compute norm.
         * \return double
         */
        template < typename T >
        double norm(Data< T > data);
        /**
         * \brief featureSpaceNorm Computes the norm in the feature space (Dual).
         * \param data Dataset to compute norm.
         * \return double
         */
        template < typename T >
        double featureSpaceNorm(std::shared_ptr<Data< T > > data);
    };

    template < typename T >
    void Kernel::compute(const std::shared_ptr<Data< T > > samples){
        size_t i, j, size = samples->getSize(), dim = samples->getDim();

        if(computed) return;
        K.assign(size, std::vector<double>(size, 0.0));

        //Calculating Matrix
        for(i = 0; i < size; ++i){
            for(j = i; j < size; ++j){
                K[i][j] = function((*samples)[i], (*samples)[j], dim);
                K[j][i] = K[i][j];
            }
        }
        computed = true;
    }

    template < typename T >
    mltk::dMatrix* Kernel::generateMatrixH(const std::shared_ptr<Data< T > > samples) {
        int i = 0, j = 0;
        size_t size = samples->getSize(), dim = samples->getDim();

        H.resize(size, std::vector<double>(size));

        /* Calculating Matrix */
        for(i = 0; i < size; ++i) {
            for (j = i; j < size; ++j) {
                H[i][j] = function(samples->getPoint(i), samples->getPoint(j), dim) * samples->getPoint(i)->Y() *
                        samples->getPoint(j)->Y();
                H[j][i] = H[i][j];
            }
        }
        std::clog << "\nH matrix generated.\n";
        return &H;
    }

    template < typename T >
    mltk::dMatrix* Kernel::generateMatrixHwithoutDim(const std::shared_ptr<Data< T > > samples, int dim) {
        int i = 0, j = 0;
        size_t size = samples->getSize();

        HwithoutDim.resize(size, std::vector<double>(size));

        /* Calculating Matrix */
        for(i = 0; i < size; ++i) {
            for (j = i; j < size; ++j) {
                HwithoutDim[i][j] = functionWithoutDim((*samples)[i], (*samples)[j], dim, samples->getDim()) *
                                    samples->getPoint(i)->Y() * samples->getPoint(j)->Y();
                HwithoutDim[j][i] = HwithoutDim[i][j];
            }
        }
    // clog << "\nH matrix without dim generated.\n";
        return &HwithoutDim;
    }

    template < typename T >
    double Kernel::function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim){
        int i = 0;
        double t, sum = 0.0;
        std::vector< T > a = one->X(), b = two->X();

        // a.erase(a.end());
        //b.erase(b.end());

        switch(type)
        {
            case 0: //Produto Interno
                for(i = 0; i < dim; ++i)
                    sum += a[i] * b[i];
                break;
            case 1: //Polinomial
                for(i = 0; i < dim; ++i)
                    sum += a[i] * b[i];
                //    sum = (param > 1) ? pow(sum+1, param) : sum;
                sum = (param > 1) ? pow(sum, param) : sum;
                break;

            case 2: //Gaussiano
                for(i = 0; i < dim; ++i)
                { t = a[i] - b[i]; sum += t * t; }
                sum = exp(-1 * sum * param);
                break;
        }

        /*The '+1' here accounts for the bias term "b" in SVM formulation since
        <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/

        return sum;// + 1.0f;
    }

    template < typename T >
    double Kernel::functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim) {
        int i = 0;
        double t, sum = 0.0;

        switch(type)
        {
            case 0: //Produto Interno
                for(i = 0; i < dim; ++i)
                    if(i != j)
                        sum += one->X()[i] * two->X()[i];
                break;

            case 1: //Polinomial
                for(i = 0; i < dim; ++i)
                    if(i != j)
                        sum += one->X()[i] * two->X()[i];
                sum = (param > 1) ? pow(sum+1, param) : sum;
                break;

            case 2: //Gaussiano
                for(i = 0; i < dim; ++i)
                    if(i != j)
                    { t = one->X()[i] - two->X()[i]; sum += t * t; }
                sum = exp(-1 * sum * param);
                break;
        }
        /*The '+1' here accounts for the bias term "b" in SVM formulation since
        <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
        return sum;// + 1.0f;
    }

    template < typename T >
    double Kernel::norm(Data< T > data){
        size_t i, j, size = data.getSize();
        double sum, sum1;
        auto points = data.getPoints();

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
    double Kernel::featureSpaceNorm(std::shared_ptr<Data< T > > data) {
        size_t i = 0, j = 0, size = data->getSize();
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
}

#endif
