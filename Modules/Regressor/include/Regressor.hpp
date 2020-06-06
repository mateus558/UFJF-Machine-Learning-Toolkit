//
// Created by mateus558 on 18/10/18.
//

#ifndef UFJF_MLTK_REGRESSOR_H
#define UFJF_MLTK_REGRESSOR_H

#endif //UFJF_MLTK_REGRESSOR_H

#include "Learner.hpp"

template <typename T >
class Regressor: public Learner<T>{
protected:
    ///Regressor solution.
    Solution solution;
public:
    /*********************************************
     *               Getters                     *
     *********************************************/

    /**
     * \brief getSolution Returns the solution of the regressor.
     * \return Solution
     */
    Solution getSolution() const {return solution;}
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
    void setW(std::vector<double> w) {this->solution.w = w;}
    /**
     * \brief setSolution Set a solution for the regressor.
     * \param solution Solution to be set.
     */
    void setSolution(Solution solution) {this->solution = solution;}
};