#pragma once
#ifndef DISTANCEMETRIC_HPP_INCLUDED
#define DISTANCEMETRIC_HPP_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLMetric __declspec(dllexport)
  #else
    #define DLLMetric __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLMetric __attribute__ ((visibility ("default")))
  #else
      #define DLLMetric
  #endif
#endif

#include "Point.hpp"
#include <cmath>

namespace mltk::metrics::dist{
        /**
         * \brief Base functor class for the implementation of new metrics metrics.
         */
        template<typename T>
        class DLLMetric  DistanceMetric {
        protected:
            std::string m_family, m_name;
        public:
            DistanceMetric() = default;

            std::string& family() { return m_family; }
            std::string& name() { return m_name; }

            virtual T operator()(const Point <T> &p1, const Point <T> &p2) const = 0;
        };

        // Lp Minkowski metrics measures

        /**
         * \brief Functor for the computation of the euclidean metrics between two points.
         */
        template<typename T>
        class DLLMetric Euclidean : public DistanceMetric<T> {
        public:
            Euclidean(){
                this->m_family = "Lp Minkowski distances";
                this->m_name = "Euclidean";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return sqrt(mltk::pow(p1 - p2, 2).sum());
            }
        };

        template<typename T>
        class DLLMetric Manhattan : public DistanceMetric<T> {
        public:
            Manhattan(){
                this->m_family = "Lp Minkowski distances";
                this->m_name = "Manhattan";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::abs(p1 - p2).sum();
            }
        };

        template<typename T>
        class DLLMetric Chebyshev : public DistanceMetric<T> {
        public:
            Chebyshev(){
                this->m_family = "Lp Minkowski distances";
                this->m_name = "Chebyshev";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::max(mltk::abs(p1 - p2));
            }
        };

        // L1 Distance measures

        template<typename T>
        class DLLMetric Lorentzian : public DistanceMetric<T> {
        public:
            Lorentzian(){
                this->m_family = "L1 Distance measures";
                this->m_name = "Lorentzian";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::log(1 - mltk::abs(p1 - p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric Canberra : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::abs(p1 - p2) / (mltk::abs(p1) + mltk::abs(p2))).sum();
            }
        };

        template<typename T>
        class DLLMetric Sorensen : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::abs(p1 - p2).sum() / (p1 + p2).sum();
            }
        };

        template<typename T>
        class DLLMetric AvgManhattan : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::abs(p1 - p2).sum() / p1.size();
            }
        };

        template<typename T>
        class DLLMetric NonIntersection : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (1 / 2) * mltk::abs(p1 - p2).sum();
            }
        };

        // Inner product metrics measures

        template<typename T>
        class DLLMetric Jaccard : public DistanceMetric<T> {
        public:
            Jaccard(){
                this->m_family = "Inner product metrics measures";
                this->m_name = "Jaccard";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::pow(p1 - p2, 2).sum() /
                       (mltk::pow(p1, 2).sum() + mltk::pow(p2, 2).sum() - (p1 * p2).sum());
            }
        };

        template<typename T>
        class DLLMetric Cosine : public DistanceMetric<T> {
        public:
            Cosine(){
                this->m_family = "Inner product metrics measures";
                this->m_name = "Cosine";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return 1 - (p1 * p2).sum() / (std::sqrt(mltk::pow(p1, 2).sum()) * std::sqrt(mltk::pow(p2, 2).sum()));
            }
        };

        template<typename T>
        class DLLMetric Dice : public DistanceMetric<T> {
        public:
            Dice(){
                this->m_family = "Inner product metrics measures";
                this->m_name = "Dice";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return 1 - 2 * (p1 * p2).sum() / (mltk::pow(p1, 2).sum() + mltk::pow(p2, 2).sum());
            }
        };

        template<typename T>
        class DLLMetric Chord : public DistanceMetric<T> {
        public:
            Chord(){
                this->m_family = "Inner product metrics measures";
                this->m_name = "Chord";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::sqrt(2 - 2 * ((p1 * p2).sum() / (mltk::pow(p1, 2).sum() * mltk::pow(p2, 2).sum())));
            }
        };

        //Squared Chord distance measures

        template<typename T>
        class DLLMetric Bhattacharyya : public DistanceMetric<T> {
        public:
            Bhattacharyya(){
                this->m_family = "Squared Chord distance measures";
                this->m_name = "Bhattacharyya";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return -std::sqrt(mltk::pow(p1 * p2, 0.5).sum());
            }
        };

        template<typename T>
        class DLLMetric SquaredChord : public DistanceMetric<T> {
        public:
            SquaredChord(){
                this->m_family = "Squared Chord distance measures";
                this->m_name = "SquaredChord";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum();
            }
        };

        template<typename T>
        class DLLMetric Matusita : public DistanceMetric<T> {
        public:
            Matusita(){
                this->m_family = "Squared Chord distance measures";
                this->m_name = "Matusita";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::sqrt(mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum());
            }
        };

        template<typename T>
        class DLLMetric Hellinger : public DistanceMetric<T> {
        public:
            Hellinger(){
                this->m_family = "Squared Chord distance measures";
                this->m_name = "Hellinger";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::sqrt(2 * mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum());
            }
        };

        // Squared L2 distance measures

        template<typename T>
        class DLLMetric SquaredEuclidean : public DistanceMetric<T> {
        public:
            SquaredEuclidean(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "SquaredEuclidean";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::pow(p1 - p2, 2).sum();
            }
        };

        template<typename T>
        class DLLMetric Clark: public DistanceMetric<T> {
        public:
            Clark(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Clark";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return mltk::pow((p1 - p2)/(mltk::abs(p1) + mltk::abs(p2)), 2).sum();
            }
        };

        template<typename T>
        class DLLMetric Neyman: public DistanceMetric<T> {
        public:
            Neyman(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Neyman";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::pow(p1 - p2, 2)/p1).sum();
            }
        };

        template<typename T>
        class DLLMetric Pearson: public DistanceMetric<T> {
        public:
            Pearson(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Pearson";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::pow(p1 - p2, 2)/p2).sum();
            }
        };

        template<typename T>
        class DLLMetric Squared: public DistanceMetric<T> {
        public:
            Squared(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Pearson";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::pow(p1 - p2, 2)/(p1 + p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric ProbabilisticSymmetric: public DistanceMetric<T> {
        public:
            ProbabilisticSymmetric(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "ProbabilisticSymmetric";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return 2 * (mltk::pow(p1 - p2, 2)/(p1 + p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric Divergence: public DistanceMetric<T> {
        public:
            Divergence(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Divergence";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return 2 * (mltk::pow(p1 - p2, 2)/mltk::pow(p1 + p2, 2)).sum();
            }
        };

        template<typename T>
        class DLLMetric AdditiveSymmetric: public DistanceMetric<T> {
        public:
            AdditiveSymmetric(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "AdditiveSymmetric";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return 2 * ((mltk::pow(p1 - p2, 2) * (p1 + p2))/(p1 * p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric Average: public DistanceMetric<T> {
        public:
            Average(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "Average";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::sqrt((1/p1.size())*(mltk::pow(p1 - p2, 2)).sum());
            }
        };

//        template<typename T>
//        class DLLMetric MeanCensoredEuclidean: public DistanceMetric<T> {
//        public:
//            T operator()(const Point <T> &p1, const Point <T> &p2) const {
//                return mltk::pow(p1-p2,2)/;
//            }
//        };

        template<typename T>
        class DLLMetric SquaredChiSquared: public DistanceMetric<T> {
        public:
            SquaredChiSquared(){
                this->m_family = "Squared L2 distance measures";
                this->m_name = "SquaredChiSquared";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::pow(p1 - p2, 2)/mltk::abs(p1 + p2)).sum();
            }
        };

        // Shannon entropy distance measures

        template<typename T>
        class DLLMetric KullbackLeibler: public DistanceMetric<T> {
        public:
            KullbackLeibler(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "KullbackLeibler";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (p1 * mltk::log(p1/p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric Jeffreys: public DistanceMetric<T> {
        public:
            Jeffreys(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "Jeffreys";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return ((p1 - p2) * mltk::log(p1/p2)).sum();
            }
        };

        template<typename T>
        class DLLMetric KDivergence: public DistanceMetric<T> {
        public:
            KDivergence(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "KDivergence";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (p1 * mltk::log((2 * p1)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class DLLMetric Topsoe: public DistanceMetric<T> {
        public:
            Topsoe(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "Topsoe";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (p1 * mltk::log((2 * p1)/(p1 + p2))).sum() + (p2 * mltk::log((2 * p2)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class DLLMetric JensenShannon: public DistanceMetric<T> {
        public:
            JensenShannon(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "JensenShannon";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (1/2) * (p1 * mltk::log((2 * p1)/(p1 + p2))).sum() + (p2 * mltk::log((2 * p2)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class DLLMetric JensenDifference: public DistanceMetric<T> {
        public:
            JensenDifference(){
                this->m_family = "Shannon entropy distance measures";
                this->m_name = "JensenDifference";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (1/2) * ((p1 * mltk::log(p1) + p2 * mltk::log(p2))/2 - ((p1 + p2)/2) * mltk::log((p1 + p2)/2)).sum();
            }
        };

        // Vicissitude distance measures

//        template<typename T>
//        class DLLMetric VicisWaveHedges: public DistanceMetric<T> {
//        public:
//            T operator()(const Point <T> &p1, const Point <T> &p2) const {
//                return (mltk::abs(p1 - p2)/).sum();
//            }
//        };

        template<typename T>
        class DLLMetric MaxSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::max((mltk::pow(p1 - p2, 2)/p1).sum(), (mltk::pow(p1 - p2, 2)/p2).sum());
            }
        };

        template<typename T>
        class DLLMetric MinSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return std::max((mltk::pow(p1 - p2, 2)/p1).sum(), (mltk::pow(p1 - p2, 2)/p2).sum());
            }
        };

        // Other distance measures

        template<typename T>
        class DLLMetric AverageL1Linf: public DistanceMetric<T> {
        public:
            AverageL1Linf(){
                this->m_family = "Other distance measures";
                this->m_name = "AverageL1Linf";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::abs(p1-p2).sum() + mltk::max(mltk::abs(p1-p2)))/2;
            }
        };

        template<typename T>
        class DLLMetric KumarJohnson: public DistanceMetric<T> {
        public:
            KumarJohnson(){
                this->m_family = "Other distance measures";
                this->m_name = "KumarJohnson";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (mltk::pow(mltk::pow(p1, 2) + mltk::pow(p2, 2), 2)/(2 * mltk::pow(p1 * p2, 3/2))).sum();
            }
        };

        template<typename T>
        class DLLMetric Taneja: public DistanceMetric<T> {
        public:
            Taneja(){
                this->m_family = "Other distance measures";
                this->m_name = "Taneja";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                return (((p1 + p2)/2) * mltk::log((p1 + p2)/(2*mltk::pow(p1 * p2, 0.5)))).sum();
            }
        };

        template<typename T>
        class DLLMetric Hassanat: public DistanceMetric<T> {
        public:
            Hassanat(){
                this->m_family = "Other distance measures";
                this->m_name = "Hassanat";
            }
            T operator()(const Point <T> &p1, const Point <T> &p2) const {
                T sum = 0;
                for(size_t i = 0; i < p1.size(); i++){
                    auto _min = std::min(p1[i], p2[i]);
                    if(_min >= 0){
                        sum += 1 - (1+_min)/(1+std::max(p1[i], p2[i]));
                    }else{
                        auto _max = std::max(p1[i], p2[i]);
                        sum += 1 - (1+_min + std::abs(_min))/(1+_max + std::abs(_max));
                    }
                }
                return sum;
            }
        };
    }

#endif