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

        template<typename T>
        LMSPrimal<T>::LMSPrimal(const Data<T>& samples, double rate, int verbose,
                                Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            this->rate = rate;
            this->verbose = verbose;

            if (initial_solution) {
                this->solution = *initial_solution;
            } else {
                this->solution.w.resize(samples.dim(), 0.0);
                this->solution.bias = 0.0;
            }
        }

        template<typename T>
        bool LMSPrimal<T>::train() {
            size_t j = 0, k = 0, size = this->samples->size(), dim = this->samples->dim();
            size_t time = this->start_time + this->max_time;
            double diff = 0.0, diffAnt = 0.0, cost_func = 0.0;
            std::vector<double> func(size, 0.0);

            if (this->verbose) {
                std::cout << "------------------------------------------------------------------------------------\n";
                std::cout << " steps     updates              cost_function              diff          secs\n";
                std::cout << "------------------------------------------------------------------------------------\n";
            }

            this->timer.reset();
            this->solution.w.assign(dim, 0.0);
            mltk::Point<> w(this->samples->dim());
            for (this->steps = 0; this->steps < this->MAX_IT; this->steps++) {
                cost_func = 0;
                for (j = 0; j < size; j++) {
                    auto input = (*this->samples)[j];
                    //Compute function
                    for (func[j] = this->solution.bias, k = 0; k < dim; k++) {
                        func[j] += (*input)[k] * w[k];
                    }
                    cost_func += (input->Y() - func[j]) * (input->Y() - func[j]);
                    //Verify if the point is a error
                    auto error = (input->Y() - func[j]);
                    w += this->rate * error * (*input);
                    this->solution.bias += (input->Y() - func[j]);
                    ++this->ctot;
                }
                cost_func *= 0.5;
                double secs = this->timer.elapsed();
                if (this->verbose) {
                    diff = fabs(cost_func - diffAnt);
                    std::cout << " " << this->steps << "           " << this->ctot << "                   " << cost_func
                              << "            " << diff << "           " << secs << "\n";
                }
                if (fabs(cost_func - diffAnt) <= this->EPS) break;
                diffAnt = cost_func;
                if (time - this->timer.elapsed() <= 0) break;
            }
            for(int i = 0; i < dim; i++){
                this->solution.w[i] = w[i];
            }
            if (this->verbose >= 1) {
                std::cout << "Number of steps through data: " << this->steps << std::endl;
                std::cout << "Number of updates: " << this->ctot << std::endl;

                if (this->verbose >= 2) {
                    for (j = 0; j < dim; ++j)
                        std::cout << "W[" << j << "]: " << this->solution.w[j]
                                  << "\n";
                    std::cout << "Bias: " << this->solution.bias << "\n\n";
                }
            }

            return true;
        }
    }
}
#endif //UFJF_MLTK_LMS_H
