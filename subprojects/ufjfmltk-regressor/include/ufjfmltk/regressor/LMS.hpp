//
// Created by mateus558 on 03/01/19.
//

#ifndef UFJF_MLTK_LMS_H
#define UFJF_MLTK_LMS_H

#include "PrimalRegressor.hpp"

namespace mltk{
    namespace regressor {
        /**
         * \brief Wrapper for the implementation of the Least Mean Square primal algorithm.
         */
        template<typename T = double>
        class LMSPrimal : public PrimalRegressor<T> {
        public:
            LMSPrimal() = default;
            explicit LMSPrimal(const Data<T>& samples, double rate = 0.5, int verbose = 0,
                               Solution *initial_solution = nullptr);

            bool train() override;
        };
    }
}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../../../src/LMS.cpp"
#endif
#endif //UFJF_MLTK_LMS_H
