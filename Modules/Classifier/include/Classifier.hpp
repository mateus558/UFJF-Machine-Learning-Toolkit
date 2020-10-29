/*! Classifier base class.
   \file Classifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef CLASSIFIER__HPP
#define CLASSIFIER__HPP

#include "../../Core/include/Learner.hpp"

template < typename T >
class Classifier: public Learner< T > {
    // Attributes
protected :
    /// Inform if there's an initial solution.
    bool hasInitialSolution = false;
    /// Support vectors points.
    std::vector<Point< T > > svs;
    /// Classifier solution.
    Solution solution;
    /// Classifier margin.
    double gamma = 0;
    
    // Operations
public :

    Classifier< T >(){} 

    Classifier< T >(const Classifier< T > &classifier)
    : Learner< T > (classifier)
    {
        this->hasInitialSolution = classifier.hasInitialSolution;
        this->gamma = classifier.gamma;
    }

    /*********************************************
     *               Getters                     *
     *********************************************/

    /**
     * \brief getSolution Returns the solution of the classifier.
     * \return Solution
     */
    Solution getSolution() const {return solution;}
    /**
     * \brief getSolution Returns a reference to the solution of the classifier.
     * \return Solution
     */
    Solution *getSolutionRef() { return &solution; }

    /*********************************************
     *               Setters                     *
     *********************************************/

    /**
    * \brief Set the gamma (margin) of the classifier.
    * \param gamma Gamma (margin) of the classifier.
    */
    inline void setGamma(double gamma) { this->gamma = gamma;}
    /**
    * \brief setW Set the weights vector of the classifier.
    * \param w weights vector.
    */
    void setW(std::vector<double> w) {this->solution.w = w;}
    /**
     * \brief setSolution Set a solution for the classifier.
     * \param solution Solution to be set.
     */
    void setSolution(Solution solution) {this->solution = solution;}
};

#endif
