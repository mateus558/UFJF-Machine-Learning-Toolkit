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
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLRandom __declspec(dllexport)
    #define DLLRandomDecl __cdecl
  #else
    #define DLLRandom 
    #define DLLRandomDecl
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLRandom __attribute__ ((visibility ("default")))
      #define DLLRandomDecl
  #else
      #define DLLRandom
      #define DLLRandomDecl
  #endif
#endif

#include <random>
#include <functional>

namespace mltk{
    /**
     * \brief Namespace for random number generation (Mersenne Twister pseudorandom generator).
     */
    namespace random {
        /// Mersenne twister generator object.
        static std::mt19937 generator;
        /// Seed used.
        static unsigned int m_seed;

        /**
         * \brief Initialize the mersenne twister pseudorandom number generator.
         * \param seed Seed used to generate the numbers.
         * \return unsigned int
         */
        inline DLLRandom auto DLLRandomDecl init(unsigned int seed = 666) {
            if(seed != m_seed){
                m_seed = (seed == 666)?std::random_device {} (): seed;
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
        inline DLLRandom Integral DLLRandomDecl intInRange(Integral low, Integral1 high) {
            Distribution dist(low, high);

            return dist(generator);
        }
        /**
         * \brief Returns a float between low and high.
         * \param low Lowest possible float.
         * \param high Highest possible float.
         * \return float
         */
        template < typename Real = double, typename Real1,
                typename Distribution = std::uniform_real_distribution<Real> >
        inline DLLRandom Real DLLRandomDecl floatInRange(Real low, Real1 high) {
            Distribution dist(low, high);

            return dist(generator);
        }
        /**
         * \brief Get the seed used in the mersenne twister.
         * \return unsigned int
         */
        inline DLLRandom auto DLLRandomDecl getSeed(){
            return m_seed;
        }
    }
}

#endif //DONKEY_KONG_TRAB_RANDOM_H
