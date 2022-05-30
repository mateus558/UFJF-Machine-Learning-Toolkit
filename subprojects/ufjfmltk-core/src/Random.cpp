//
// Created by Marim on 14/07/2021.
//

#include <iostream>
#include "ufjfmltk/core/Random.hpp"

namespace mltk::random {
    unsigned int seed_param = 1;
    std::mt19937 m_generator{std::random_device{}()};

    size_t  init(unsigned int seed) {
        if(seed != seed_param){
            m_generator.discard(std::mt19937::state_size);
            seed_param = seed;
            m_seed = (seed == 0)?std::random_device {} (): seed;
            m_generator.seed(m_seed);
        }
        return m_seed;
    }

    size_t  getSeed(){
        return m_seed;
    }

    template<typename Integral, typename Integral1, typename Distribution>
    Integral intInRange(Integral low, Integral1 high){
        Distribution dist(low, high);

        return dist(m_generator);
    }

    template<typename Real, typename Distribution >
    Real floatInRange(Real low, Real high) {
        Distribution dist(low, high);

        return dist(m_generator);
    }

    template double floatInRange<double, std::uniform_real_distribution<double> >(double, double);
    template double floatInRange<double, std::normal_distribution<double> >(double, double);
    template float floatInRange<float, std::uniform_real_distribution<float> >(float, float);
    template float floatInRange<float, std::normal_distribution<float> >(float, float);
    template double floatInRange<double, std::gamma_distribution<double> >(double, double);
    template float floatInRange<float, std::gamma_distribution<float> >(float, float);
    template double floatInRange<double, std::fisher_f_distribution<double> >(double, double);
    template float floatInRange<float, std::fisher_f_distribution<float> >(float, float);
    template int intInRange<int, int, std::uniform_int_distribution<int> >(int, int);
    template size_t intInRange<size_t, size_t, std::uniform_int_distribution<size_t> >(size_t, size_t);
    template int intInRange<int, unsigned long, std::uniform_int_distribution<int> >(int, unsigned long);
    template int intInRange<int, unsigned int, std::uniform_int_distribution<int> >(int, unsigned int);
}
