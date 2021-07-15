/*! Statistics wrapper class
   \file Statistics.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#ifndef STATISTICS__HPP
#define STATISTICS__HPP

#include <vector>
#include <cmath>
#include <iostream>
#include "Data.hpp"

namespace mltk{
    template < typename T > class Data;
}

//! Namespace for statistical methods.
namespace mltk::stats {
    /**
     * \brief Compute the mean (average) of a point.
     * \param p Point to compute the mean.
     * \return double
     */
    template < typename T, typename R >
    double mean (const mltk::Point<T, R> &p);
    /**
     * \brief Compute the mean (average) of a feature in data.
     * \param feat Feature to compute the mean.
     * \return double
     */
    template <typename T>
    double mean(const Data<T>& data, size_t feat);
    /**
     * \brief Compute the standard deviation of a point.
     * \param p Point to compute the mean.
     * \return double
     */
    template < typename T, typename R >
    double std_dev(const mltk::Point<T, R> &p);
    /**
     * \brief Compute the standard deviation of a feature in data.
     * \param feat Feature to compute the standard deviation.
     * \return double
     */
    template < typename T >
    double std_dev(const Data<T>& data, size_t feat);
    /**
     * \brief Compute the variance of a point.
     * \param p Point to compute the variance.
     * \return double
     */
    template < typename T, typename R >
    double var(const mltk::Point<T, R> &p);
    /**
     * \brief Compute the variance of a feature in data.
     * \param feat Feature to compute the variance.
     * \return double
     */
    template <typename T>
    double var(const Data<T>& data, size_t feat);
    /**
     * \brief Compute the covariance between two points.
     * \param p First point to compute the variance.
     * \param p1 second point to compute the variance.
     * \return double
     */
    template < typename T, typename R >
    double covar(const mltk::Point<T, R> &p, const mltk::Point<T, R> &p1);
    /**
     * \brief Returns radius of the ball that circ. the data.
     * \param data  Dataset to compute the radius.
     * \param feat Feature to be ignored (-1 uses all features).
     * \param q Lp-Norm to be used.
     * \return double
     */
    template < typename T >
    double radius(const Data<T>& data, int feat, double q);
    /**
     * \brief Compute the distance between the centers of binary classes without given features.
     * \param data  Dataset to compute the metrics.
     * \param feats Features to be excluded from the computation.
     * \param index Feature to be ignored (-1 uses all features).
     * \return double
     */
    template < typename T >
    double distCenters(const Data<T>& data, int feat);
    /**
     * \brief Compute the distance between the centers of binary classes without given features.
     * \param data  Dataset to compute the metrics.
     * \param feats Features to be excluded from the computation.
     * \param index Feature to be ignored (-1 uses all features).
     * \return double
     */
    template < typename T >
    double distCentersWithoutFeats(const Data<T>& data, const std::vector<int>& feats, int index);

    /*********************************************
     *               Implementation              *
     *********************************************/

    template < typename T, typename R >
    double mean (const Point<T, R> &p){
        assert(p.size() > 0);
        return p.sum()/p.size();
    }

    template <typename T>
    double mean(const Data<T>& data, size_t feat){
        assert(feat < data.dim());
        double sum = 0.0;
        for(size_t i = 0; i < data.size(); i++){
            sum += (*data[i])[feat];
        }
        return (data.size()>0)?sum/data.size():0.0;
    }

    template < typename T, typename R >
    double std_dev(const Point<T, R> &p){
        assert(p.size() > 0);
        return std::sqrt((mltk::pow(p-mltk::stats::mean(p), 2)).sum()/p.size());
    }

    template < typename T >
    double std_dev(const Data<T>& data, size_t feat){
        int i, size = data.size();
        double avg, sd;
        std::vector<std::shared_ptr<Point< T > > > points = data.points();

        if(size == 1) return 0.0;

        avg = mltk::stats::mean(data, feat);

        for(sd = 0.0, i = 0; i < data.size(); ++i){
            sd += (points[i]->X()[feat] - avg)*(points[i]->X()[feat] - avg);
        }

        return std::sqrt(sd/(data.size() - 1));
    }

    template < typename T, typename R >
    double var(const Point<T, R> &p){
        assert(p.size() > 0);
        return mltk::pow(p-mltk::stats::mean(p), 2).sum()/p.size();
    }

    template <typename T>
    double var(const Data<T>& data, size_t feat){
        int i, j;
        int dim = data.dim(), size = data.size();
        std::vector<int> fnames = data.getFeaturesNames();
        std::vector<double> avg(dim);
        std::vector<std::shared_ptr<Point< T > > > points = data.points();

        for(j = 0; j < dim; ++j){
            if(feat < 0 || fnames[j] != feat){
                avg[j] = 0.0;
                for(i = 0; i < size; ++i){
                    avg[j] += points[i]->X()[j];
                }
                avg[j] = avg[j] / size;
            }
        }

        double sum = 0.0;
        for(i = 0; i < size; ++i){
            double norm = 0.0;
            for(j = 0; j < dim; ++j){
                if(feat < 0 || fnames[j] != feat){
                    norm += std::pow(avg[j] - points[i]->X()[j], 2);
                }
            }
            sum += norm;
        }
        sum = sum/size;
        return sum;
    }

    template < typename T, typename R >
    double covar(const Point<T, R> &p, const Point<T, R> &p1){
        assert(p.size() == p1.size());
        return ((p-mltk::stats::mean(p))*(p1-mltk::stats::mean(p1))).sum()/(p1.size()-1.0);
    }

    template < typename T >
    double radius(const Data<T>& data, int feat, double q){
        int i = 0, j = 0, dim = data.dim(), size = data.size();
        double norm = 0.0;
        double max = 1.0;
        std::vector<int> fnames = data.getFeaturesNames();
        std::vector<double> avg(dim, 0.0);
        std::vector<std::shared_ptr<Point< T > > > points = data.points();

        if(q == 2){
            for(j = 0; j < dim; ++j){
                if(feat < 0 || fnames[j] != feat){
                    avg[j] = 0.0;
                    for(i = 0; i < size; ++i){
                        avg[j] += points[i]->X()[j];
                    }
                    avg[j] = avg[j] / size;
                }
            }

            for(max = 0, i = 0; i < size; ++i){
                for(norm = 0, j = 0; j < dim; ++j){
                    if(feat < 0 || fnames[j] != feat){
                        norm += std::pow(avg[j] - points[i]->X()[j], 2);
                    }
                }
                norm = std::sqrt(norm);
                if(max < norm) max = norm;
            }

        }else if(q == 1){
            for(max = 0, i = 0; i < size; ++i){
                for(j = 0; j < dim; ++j){
                    if(feat < 0 || fnames[j] != feat)
                        if(max < fabs(points[i]->X()[j]))
                            max = fabs(points[i]->X()[j]);
                }
            }
        }

        return max;
    }

    template < typename T >
    double distCenters(const Data<T>& data, int feat){
        int i = 0, j = 0, dim = data.dim(), size = data.size();
        double dist = 0.0;
        int size_pos = 0, size_neg = 0;
        std::vector<int> fnames = data.getFeaturesNames();
        std::vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
        std::vector<std::shared_ptr<Point< T > > > points = data.points();

        for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
            if(points[i]->Y() == 1)	size_pos++;
            else 					size_neg++;
        }

        for(j = 0; j < dim; ++j){
            for(i = 0; i < size; ++i){
                if(points[i]->Y() == 1){
                    avg_pos[j] += points[i]->X()[j];
                }else
                    avg_neg[j] += points[i]->X()[j];
            }

            avg_pos[j] /= (double)size_pos;
            avg_neg[j] /= (double)size_neg;
        }

        for(dist = 0.0, j = 0; j < dim; ++j){
            if(feat < 0 || fnames[j] != feat)
                dist += std::pow(avg_pos[j] - avg_neg[j], 2);
        }

        return std::sqrt(dist);
    }

    template < typename T >
    double distCentersWithoutFeats(const Data<T>& data, const std::vector<int>& feats, int index){
        int i = 0, j = 0, dim = data.dim(), size = data.size();
        double dist = 0.0;
        int size_pos = 0, size_neg = 0, featsize = feats.size();
        std::vector<int> fnames = data.getFeaturesNames();
        std::vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
        std::vector<std::shared_ptr<Point< T > > > points = data.points();

        for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
            if(points[i]->Y() == 1)	size_pos++;
            else					size_neg++;
        }

        for(j = 0; j < dim; ++j){
            for(i = 0; i < size; ++i){
                if(points[i]->Y() == 1)
                    avg_pos[j] += points[i]->X()[j];
                else
                    avg_neg[j] += points[i]->X()[j];
            }

            avg_pos[j] /= (double) size_pos;
            avg_neg[j] /= (double) size_neg;
        }

        for(dist = 0.0, j = 0; j < dim; ++j){
            for(i = 0; i < featsize; ++i){
                if(fnames[j] == feats[i])
                    dist -= std::pow(avg_pos[j] - avg_neg[j], 2);
            }
        }

        return std::sqrt(std::fabs(dist));
    }
}


#endif
