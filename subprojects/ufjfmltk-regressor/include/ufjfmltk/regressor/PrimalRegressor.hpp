//
// Created by mateus558 on 03/11/18.
//

#pragma once

#include "Regressor.hpp"

namespace mltk::regressor {
            template<typename T>
            class PrimalRegressor : public Regressor<T> {
                // Attributes
            protected :
                /// Weights vector.
                std::vector<double> w;

            public:

                PrimalRegressor() = default;

                explicit PrimalRegressor(DataPointer<T> samples) : Regressor<T>(samples) {}

                double evaluate(const Point<T> &p, bool raw_value = false) override {
                    double func = 0.0;
                    size_t i, dim = this->solution.w.size();

                    if (p.X().size() != dim) {
                        std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
                        return 0;
                    }

                    for (func = this->solution.bias, i = 0; i < dim; i++) {
                        func += this->solution.w[i] * p[i];
                    }

                    return func;
                }

                /*********************************************
                 *               Getters                     *
                 *********************************************/

                std::string getFormulationString() override { return "Primal"; }

                /*********************************************
                 *               Setters                     *
                 *********************************************/

            };
        }
