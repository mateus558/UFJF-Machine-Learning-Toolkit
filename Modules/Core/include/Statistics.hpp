/*! Statistics wrapper class
   \file Statistics.hpp
   \author Mateus Coutinho Marim
*/

#ifndef STATISTICS__HPP
#define STATISTICS__HPP
#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "Point.hpp"
#include "Data.hpp"

namespace mltk{
    template < typename T > class Data;

    /**
     * \brief Class with methods for statistical computations.
     */

    template  < typename T >
    class Statistics {
        // Attributes
        friend class Data< T >;
    private :
        /// Centroid of all the dataset.
        Point< T > centroid;
        /// Centroid of the positive points.
        Point< T > pos_centroid;
        /// Centroid of the negative points.
        Point< T > neg_centroid;
        /// Number of positive points.
        size_t n_pos = 0;
        /// Number of negative points.
        size_t n_neg = 0;
    // Operations
    public:
        Statistics (){}
        /**
         * \brief Compute the mean (average) of a vector.
         * \param p (???) Point to compute the mean.
         * \return double
         */
        static double mean (std::vector< T > p);
        /**
         * \brief Computes the mean of a feature in the sample.
         * \param data (???) Sample where the feature is located.
         * \param index (???) Index of the feature to compute the mean.
         * \return double
         */
        static double getFeatureMean (std::shared_ptr<Data< T > > data, int index);
        /**
         * \brief Compute the variance of a vector.
         * \param p (???) Vector to compute the variance.
         * \return double
         */
        static double variance (std::vector< T > p);
        /**
         * \brief Compute the variance of a sample.
         * \param data (???) Sample to compute the variance.
         * \param index (???) Index of the feature to be ignored. (-1 dont ignore any feature)
         * \return double
         */
        static double variance (std::shared_ptr<Data< T > > data, int index);
        /**
         * \brief Compute the standard deviation of a vector.
         * \param p (???) Point to compute stdev.
         * \return double
         */
        static double stdev (std::vector< T > p);
        /**
         * \brief Computes the standard deviation of a feature.
         * \param data (???) Sample where the feature is located.
         * \param index (???) Index of teh feature to compute the standard deviation.
         * \return double
         */
        static double getFeatureStdev (std::shared_ptr<Data< T > > data, int index);
        /**
         * \brief Returns radius of the ball that circ. the data.
         * \param data  Dataset to compute the radius.
         * \param index Feature to be ignored (-1 uses all features).
         * \param q Lp-Norm to be used.
         * \return double
         */
        static double getRadius(std::shared_ptr<Data< T > > data, int index, double q);
        /**
         * \brief Returns metrics of centers of the classes.
         * \param data  Dataset to compute the metrics.
         * \param index Feature to be ignored (-1 uses all features).
         * \return double
         */
        static double getDistCenters(std::shared_ptr<Data< T > > data, int index);
        /**
         * \brief Returns metrics of centers of the classes without given features.
         * \param data  Dataset to compute the metrics.
         * \param feats Features to be excluded from the computation.
         * \param index Feature to be ignored (-1 uses all features).
         * \return double
         */
        static double getDistCentersWithoutFeats(std::shared_ptr<Data< T > > data, std::vector<int> feats, int index);
    };
}
#endif
