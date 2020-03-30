/*! Solution wrapper class
   \file Solution.hpp
   \author Mateus Coutinho Marim
*/

#ifndef SOLUTION__HPP
#define SOLUTION__HPP

#include <vector>
#include "../includes/Kernel.hpp"

class Solution {
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

#endif
