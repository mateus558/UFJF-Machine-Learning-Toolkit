//
// Created by mateus558 on 06/03/2020.
//

#ifndef UFJF_MLTK_DUALREGRESSOR_HPP
#define UFJF_MLTK_DUALREGRESSOR_HPP

#include "Regressor.hpp"

namespace mltk{
    template < typename T >
    class DualRegressor: public Regressor < T > {
        // Attributes
    protected :
        /// Alphas vector.
        std::vector<double> alpha;
        /// Object for kernel computations.
        Kernel *kernel = nullptr;
    public:

        double evaluate(const Point< T >  &p, bool raw_value=false) override {
            double func = 0.0;
            size_t i, dim = this->solution.w.size();

            if(p.x.size() != dim){
                std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
                return 0;
            }

            for(func = this->solution.bias, i = 0; i < dim; i++){
                func += this->solution.w[i] * p[i];
            }

            return func;
        }

        /*********************************************
         *               Getters                     *
         *********************************************/

        std::string getFormulationString() override { return "Dual"; }

        /*********************************************
         *               Setters                     *
         *********************************************/

    };
}

#endif //UFJF_MLTK_DUALREGRESSOR_HPP
