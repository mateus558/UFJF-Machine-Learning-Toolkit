#ifndef DISTANCEMETRIC_HPP_INCLUDED
#define DISTANCEMETRIC_HPP_INCLUDED

#include "Point.hpp"
#include <cmath>

namespace mltk{
    namespace metrics{ namespace dist{
        /**
         * \brief Base functor class for the implementation of new metrics metrics.
         */
        template<typename T>
        class DistanceMetric {
        public:
            DistanceMetric() = default;

            virtual T operator()(const Point <T> &p1, const Point <T> &p2) = 0;
        };

        // Lp Minkowski metrics measures

        /**
         * \brief Functor for the computation of the euclidean metrics between two points.
         */
        template<typename T>
        class Euclidean : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return sqrt(mltk::pow(p1 - p2, 2).sum());
            }
        };

        template<typename T>
        class Manhattan : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::abs(p1 - p2).sum();
            }
        };

        template<typename T>
        class Chebyshev : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::max(mltk::abs(p1 - p2));
            }
        };

        // L1 Distance measures

        template<typename T>
        class Lorentzian : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::log(1 - mltk::abs(p1 - p2)).sum();
            }
        };

        template<typename T>
        class Canberra : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::abs(p1 - p2) / (mltk::abs(p1) + mltk::abs(p2))).sum();
            }
        };

        template<typename T>
        class Sorensen : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::abs(p1 - p2).sum() / (p1 + p2).sum();
            }
        };

        template<typename T>
        class AvgManhattan : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::abs(p1 - p2).sum() / p1.size();
            }
        };

        template<typename T>
        class NonIntersection : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (1 / 2) * mltk::abs(p1 - p2).sum();
            }
        };

        // Inner product metrics measures

        template<typename T>
        class Jaccard : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::pow(p1 - p2, 2).sum() /
                       (mltk::pow(p1, 2).sum() + mltk::pow(p2, 2).sum() - (p1 * p2).sum());
            }
        };

        template<typename T>
        class Cosine : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return 1 - (p1 * p2).sum() / (std::sqrt(mltk::pow(p1, 2).sum()) * std::sqrt(mltk::pow(p2, 2).sum()));
            }
        };

        template<typename T>
        class Dice : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return 1 - 2 * (p1 * p2).sum() / (mltk::pow(p1, 2).sum() + mltk::pow(p2, 2).sum());
            }
        };

        template<typename T>
        class Chord : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::sqrt(2 - 2 * ((p1 * p2).sum() / (mltk::pow(p1, 2).sum() * mltk::pow(p2, 2).sum())));
            }
        };

        //Squared Chord distance measures

        template<typename T>
        class Bhattacharyya : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return -std::sqrt(mltk::pow(p1 * p2, 0.5).sum());
            }
        };

        template<typename T>
        class SquaredChord : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum();
            }
        };

        template<typename T>
        class Matusita : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::sqrt(mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum());
            }
        };

        template<typename T>
        class Hellinger : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::sqrt(2 * mltk::pow(mltk::pow(p1, 0.5) - mltk::pow(p2, 0.5), 2).sum());
            }
        };

        // Squared L2 distance measures

        template<typename T>
        class SquaredEuclidean : public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::pow(p1 - p2, 2).sum();
            }
        };

        template<typename T>
        class Clark: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return mltk::pow((p1 - p2)/(mltk::abs(p1) + mltk::abs(p2)), 2).sum();
            }
        };

        template<typename T>
        class Neyman: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::pow(p1 - p2, 2)/p1).sum();
            }
        };

        template<typename T>
        class Pearson: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::pow(p1 - p2, 2)/p2).sum();
            }
        };

        template<typename T>
        class Squared: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::pow(p1 - p2, 2)/(p1 + p2)).sum();
            }
        };

        template<typename T>
        class ProbabilisticSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return 2 * (mltk::pow(p1 - p2, 2)/(p1 + p2)).sum();
            }
        };

        template<typename T>
        class Divergence: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return 2 * (mltk::pow(p1 - p2, 2)/mltk::pow(p1 + p2, 2)).sum();
            }
        };

        template<typename T>
        class AdditiveSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return 2 * ((mltk::pow(p1 - p2, 2) * (p1 + p2))/(p1 * p2)).sum();
            }
        };

        template<typename T>
        class Average: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::sqrt((1/p1.size())*(mltk::pow(p1 - p2, 2)).sum());
            }
        };

//        template<typename T>
//        class MeanCensoredEuclidean: public DistanceMetric<T> {
//        public:
//            T operator()(const Point <T> &p1, const Point <T> &p2) {
//                return mltk::pow(p1-p2,2)/;
//            }
//        };

        template<typename T>
        class SquaredChiSquared: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::pow(p1 - p2, 2)/mltk::abs(p1 + p2)).sum();
            }
        };

        // Shannon entropy distance measures

        template<typename T>
        class KullbackLeibler: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (p1 * mltk::log(p1/p2)).sum();
            }
        };

        template<typename T>
        class Jeffreys: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return ((p1 - p2) * mltk::log(p1/p2)).sum();
            }
        };

        template<typename T>
        class KDivergence: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (p1 * mltk::log((2 * p1)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class Topsoe: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (p1 * mltk::log((2 * p1)/(p1 + p2))).sum() + (p2 * mltk::log((2 * p2)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class JensenShannon: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (1/2) * (p1 * mltk::log((2 * p1)/(p1 + p2))).sum() + (p2 * mltk::log((2 * p2)/(p1 + p2))).sum();
            }
        };

        template<typename T>
        class JensenDifference: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (1/2) * ((p1 * mltk::log(p1) + p2 * mltk::log(p2))/2 - ((p1 + p2)/2) * mltk::log((p1 + p2)/2)).sum();
            }
        };

        // Vicissitude distance measures

//        template<typename T>
//        class VicisWaveHedges: public DistanceMetric<T> {
//        public:
//            T operator()(const Point <T> &p1, const Point <T> &p2) {
//                return (mltk::abs(p1 - p2)/).sum();
//            }
//        };

        template<typename T>
        class MaxSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::max((mltk::pow(p1 - p2, 2)/p1).sum(), (mltk::pow(p1 - p2, 2)/p2).sum());
            }
        };

        template<typename T>
        class MinSymmetric: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return std::max((mltk::pow(p1 - p2, 2)/p1).sum(), (mltk::pow(p1 - p2, 2)/p2).sum());
            }
        };

        // Other distance measures

        template<typename T>
        class AverageL1Linf: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::abs(p1-p2).sum() + mltk::max(mltk::abs(p1-p2)))/2;
            }
        };

        template<typename T>
        class KumarJohnson: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (mltk::pow(mltk::pow(p1, 2) + mltk::pow(p2, 2), 2)/(2 * mltk::pow(p1 * p2, 3/2))).sum();
            }
        };

        template<typename T>
        class Taneja: public DistanceMetric<T> {
        public:
            T operator()(const Point <T> &p1, const Point <T> &p2) {
                return (((p1 + p2)/2) * mltk::log((p1 + p2)/(2*mltk::pow(p1 * p2, 0.5)))).sum();
            }
        };
    }
    }
}

#endif