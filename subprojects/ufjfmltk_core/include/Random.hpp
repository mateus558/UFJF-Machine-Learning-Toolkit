/*! Random namespace
   \file Random.hpp
   \author Mateus Coutinho Marim
*/

//
// Created by mateus558 on 25/08/17.
//

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
        std::mt19937 m_gen;
        /// Seed used.
        unsigned int m_seed;

        /**
         * \brief Initialize the mersenne twister pseudorandom number generator.
         * \param seed Seed used to generate the numbers.
         * \return unsigned int
         */
        auto init(unsigned int seed = 666) {
            m_seed = (seed == 666)?std::random_device {} (): seed;
            m_gen.seed(m_seed);

            return m_seed;
        }
        /**
         * \brief Returns a integer between low and high.
         * \param low Lowest possible integer.
         * \param high Highest possible integer.
         * \return int
         */
        int intInRange(int low, int high) {
            std::uniform_int_distribution<int> dist(low, high);

            return dist(m_gen);
        }
        /**
         * \brief Returns a float between low and high.
         * \param low Lowest possible float.
         * \param high Highest possible float.
         * \return float
         */
        float floatInRange(float low, float high) {
            std::uniform_real_distribution<float> dist(low, high);

            return dist(m_gen);
        }
        /**
         * \brief Get the seed used in the mersenne twister.
         * \return unsigned int
         */
        auto getSeed(){
            return m_seed;
        }
    }
}

#endif //DONKEY_KONG_TRAB_RANDOM_H
