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
}
