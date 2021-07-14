/*! Random namespace
   \file Random.hpp
   \author Mateus Coutinho Marim
*/

//
// Created by mateus558 on 25/08/17.
//
#pragma once
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <random>
#include <functional>

namespace mltk{
    /**
     * \brief Namespace for random number generation (Mersenne Twister pseudorandom generator).
     */
    namespace random {
        /// Seed used.
        extern unsigned int m_seed;
        extern unsigned int seed_param;
        extern std::mt19937 m_generator;

        /**
         * \brief Initialize the mersenne twister pseudorandom number generator.
         * \param seed Seed used to generate the numbers.
         * \return unsigned int
         */
        size_t init(unsigned int seed = 0);

        /**
         * \brief Returns a integer between low and high.
         * \param low Lowest possible integer.
         * \param high Highest possible integer.
         * \return int
         */
        template<typename Integral = int, typename Integral1,
                typename Distribution = std::uniform_int_distribution<Integral> >
        inline Integral intInRange(Integral low, Integral1 high){
            Distribution dist(low, high);

            return dist(m_generator);
        }

        /**
         * \brief Returns a float between low and high.
         * \param low Lowest possible float.
         * \param high Highest possible float.
         * \return float
         */
        template<typename Real = double,
                typename Distribution = std::uniform_real_distribution<Real> >
        Real floatInRange(Real low, Real high) {
            Distribution dist(low, high);

            return dist(m_generator);
        }

        /**
         * \brief Get the seed used in the mersenne twister.
         * \return unsigned int
         */
        auto getSeed();
    }
}

#endif //DONKEY_KONG_TRAB_RANDOM_H
