//
// Created by mateus558 on 29/07/19.
//

#ifndef UFJF_MLTK_ENSEMBLE_HPP
#define UFJF_MLTK_ENSEMBLE_HPP

#include "Learner.hpp"

template <typename T>
class Ensemble: public Learner< T > {
    // Attributes
protected:
    /// Ensemble solution.
    Solution solution;
public:
    /*********************************************
     *               Getters                     *
     *********************************************/

    /**
     * \brief getSolution Returns the solution of the Ensemble method.
     * \return Solution
     */
    Solution getSolution() const {return solution;}
    /**
     * \brief getSolution Returns a reference to the solution of the Ensemble method.
     * \return Solution
     */
    Solution *getSolutionRef() { return &solution; }

    /*********************************************
     *               Setters                     *
     *********************************************/
};


#endif //UFJF_MLTK_ENSEMBLE_HPP
