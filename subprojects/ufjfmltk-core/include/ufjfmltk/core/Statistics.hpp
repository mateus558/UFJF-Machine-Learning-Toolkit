/*! Statistics wrapper class
   \file Statistics.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#ifndef STATISTICS__HPP
#define STATISTICS__HPP
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLStatistics __declspec(dllexport)
  #else
    #define DLLStatistics __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLStatistics __attribute__ ((visibility ("default")))
  #else
      #define DLLStatistics
  #endif
#endif

#include <vector>
#include <cmath>
#include <iostream>
#include "Point.hpp"
#include "Data.hpp"

namespace mltk{
    template < typename T > class Data;
    /**
     * \brief Class with methods for statistical methods.
     */
    namespace stats {
        /**
         * \brief Compute the mean (average) of a point.
         * \param p Point to compute the mean.
         * \return double
         */
        template < typename T, typename R >
        T mean (const mltk::Point<T, R> &p);
        /**
         * \brief Compute the mean (average) of a feature in data.
         * \param feat Feature to compute the mean.
         * \return double
         */
        template <typename T>
        double mean(const mltk::Data<T>& data, size_t feat);
        /**
         * \brief Compute the standard deviation of a point.
         * \param p Point to compute the mean.
         * \return double
         */
        template < typename T, typename R >
        T std_dev(const mltk::Point<T, R> &p);
        /**
         * \brief Compute the standard deviation of a feature in data.
         * \param feat Feature to compute the standard deviation.
         * \return double
         */
        template < typename T >
        double std_dev(const mltk::Data<T>& data, size_t feat);
        /**
         * \brief Compute the variance of a point.
         * \param p Point to compute the variance.
         * \return double
         */
        template < typename T, typename R >
        T var(const mltk::Point<T, R> &p);
        /**
         * \brief Compute the variance of a feature in data.
         * \param feat Feature to compute the variance.
         * \return double
         */
        template <typename T>
        double var(const mltk::Data<T>& data, size_t feat);
        /**
         * \brief Compute the covariance between two points.
         * \param p First point to compute the variance.
         * \param p1 second point to compute the variance.
         * \return double
         */
        template < typename T, typename R >
        T covar(const mltk::Point<T, R> &p, const mltk::Point<T, R> &p1);
        /**
         * \brief Returns radius of the ball that circ. the data.
         * \param data  Dataset to compute the radius.
         * \param feat Feature to be ignored (-1 uses all features).
         * \param q Lp-Norm to be used.
         * \return double
         */
        template < typename T >
        double radius(const mltk::Data<T>& data, int feat, double q);
        /**
         * \brief Compute the distance between the centers of binary classes without given features.
         * \param data  Dataset to compute the metrics.
         * \param feats Features to be excluded from the computation.
         * \param index Feature to be ignored (-1 uses all features).
         * \return double
         */
        template < typename T >
        double distCenters(const mltk::Data<T>& data, int feat);
        /**
         * \brief Compute the distance between the centers of binary classes without given features.
         * \param data  Dataset to compute the metrics.
         * \param feats Features to be excluded from the computation.
         * \param index Feature to be ignored (-1 uses all features).
         * \return double
         */
        template < typename T >
        double distCentersWithoutFeats(const mltk::Data<T>& data, const std::vector<int>& feats, int index);
    }
}
#endif
