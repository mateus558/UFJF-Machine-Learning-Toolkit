//
// Created by mateus558 on 18/10/18.
//

#ifndef UFJF_MLTK_REGRESSOR_H
#define UFJF_MLTK_REGRESSOR_H

#include <ufjfmltk/core/Learner.hpp>

namespace mltk{
        /**
         * \brief Namespace for regression methods.
         */
        namespace regressor {
            
            template<typename T>
            class Regressor : public Learner<T> {
            protected:
                ///Regressor solution.
                Solution solution;
            public:
                Regressor() {}

                Regressor(DataPointer <T> samples) : Learner<T>(samples) {}

                /*********************************************
                 *               Getters                     *
                 *********************************************/

                /**
                 * \brief getSolution Returns the solution of the regressor.
                 * \return Solution
                 */
                Solution getSolution() const { return solution; }

                /**
                 * \brief getSolution Returns a reference to the solution of the regressor.
                 * \return Solution
                 */
                Solution *getSolutionRef() { return &solution; }

                /*********************************************
                 *               Setters                     *
                 *********************************************/

                /**
                * \brief setW Set the weights vector of the regressor.
                * \param w weights vector.
                */
                void setW(const std::vector<double> &w) { this->solution.w = w; }

                /**
                 * \brief setSolution Set a solution for the regressor.
                 * \param solution Solution to be set.
                 */
                void setSolution(Solution solution) { this->solution = solution; }
            };
        }
}

#endif //UFJF_MLTK_REGRESSOR_H
