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
        /// Mersenne twister generator object.
        static std::mt19937 generator{std::random_device {} ()};
        /// Seed used.
        static unsigned int m_seed = 0;

        /**
         * \brief Initialize the mersenne twister pseudorandom number generator.
         * \param seed Seed used to generate the numbers.
         * \return unsigned int
         */
        inline  auto  init(unsigned int seed = 0) {
            if(seed != m_seed){
                m_seed = (seed == 0)?std::random_device {} (): seed;
                generator.seed(m_seed);
            }
            return m_seed;
        }
        /**
         * \brief Returns a integer between low and high.
         * \param low Lowest possible integer.
         * \param high Highest possible integer.
         * \return int
         */
        template < typename Integral = int, typename Integral1,
                typename Distribution = std::uniform_int_distribution<Integral>  >
        inline  Integral  intInRange(Integral low, Integral1 high) {
            Distribution dist(low, high);

            return dist(generator);
        }
        /**
         * \brief Returns a float between low and high.
         * \param low Lowest possible float.
         * \param high Highest possible float.
         * \return float
         */
        template < typename Real = double,
                typename Distribution = std::uniform_real_distribution<Real> >
        inline  Real  floatInRange(Real low, Real high) {
            Distribution dist(low, high);

            return dist(generator);
        }
        /**
         * \brief Get the seed used in the mersenne twister.
         * \return unsigned int
         */
        inline  auto  getSeed(){
            return m_seed;
        }
    }
}

#endif //DONKEY_KONG_TRAB_RANDOM_H
