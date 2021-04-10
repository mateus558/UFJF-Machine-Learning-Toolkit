/*! Solution wrapper class
   \file Solution.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#ifndef SOLUTION__HPP
#define SOLUTION__HPP
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLSolution __declspec(dllexport)
  #else
    #define DLLSolution __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLSolution __attribute__ ((visibility ("default")))
  #else
      #define DLLSolution
  #endif
#endif

#include <vector>
#include "Kernel.hpp"

namespace mltk{
    class DLLSolution Solution {
        // Attributes
    public :
        /// Weights vector
        std::vector<double> w, func;
        ///Kernel for Dual methods
        Kernel K;
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
#endif
