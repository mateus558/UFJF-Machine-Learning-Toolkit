/*! Solution wrapper class
   \file Solution.hpp
   \author Mateus Coutinho Marim
*/


#pragma once

#include <vector>
#include "Kernel.hpp"

namespace mltk{
    class  Solution {
        // Attributes
    public :
        /// Weights vector
        mltk::Point<double> w, func;
        ///Kernel for Dual methods
        Kernel<double> K;
        /// Alpha Vector for Dual methods
        std::vector<double> alpha;
        /// Bias of the solution.
        double bias = 0;
        /// Features names of the resulting solution.
        std::vector<int> fnames;
        /// Margin generated from the classifier that generated the solution.
        double margin = 0;
        /// Norm of the solution.
        double norm = 0;
        /// Number of support Vectors
        unsigned int svs = 0;

        Solution& operator=(const Solution& other){
            w = other.w;
            func = other.func;
            K = other.K;
            alpha = other.alpha;
            bias = other.bias;
            fnames = other.fnames;
            margin = other.margin;
            norm = other.norm;
            svs = other.svs;

            return *this;
        }
    };
}
