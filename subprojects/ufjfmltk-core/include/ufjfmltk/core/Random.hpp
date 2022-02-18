/*! Random namespace
   \file Random.hpp
   \author Mateus Coutinho Marim
*/

//
// Created by mateus558 on 25/08/17.
//
#pragma once

#include <random>
#include <functional>

//! Namespace for random number generation (Mersenne Twister pseudorandom generator).
namespace mltk::random {
        /// Seed used.
        static unsigned int m_seed = 1;
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
        inline Integral intInRange(Integral low, Integral1 high);

        /**
         * \brief Returns a float between low and high.
         * \param low Lowest possible float.
         * \param high Highest possible float.
         * \return float
         */
        template<typename Real = double,
                typename Distribution = std::uniform_real_distribution<Real> >
        inline Real floatInRange(Real low, Real high);

        /**
         * \brief Get the seed used in the mersenne twister.
         * \return unsigned int
         */
        size_t getSeed();
    }
