//
// Created by mateus on 25/05/2021.
//

#ifndef UFJFMLTK_METRICS_HPP
#define UFJFMLTK_METRICS_HPP
#include <cassert>
#include "ufjfmltk/core/Point.hpp"

namespace mltk{
    namespace metrics{
        double MSE(mltk::Point<double> real, mltk::Point<double> predicted){
            assert(real.size() == predicted.size());
            return mltk::pow(real - predicted, 2).sum()/real.size();
        }
    }
}

#endif //UFJFMLTK_METRICS_HPP
