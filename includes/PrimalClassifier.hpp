/*! PrimalClassifier base class
   \file PrimalClassifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef PRIMALCLASSIFIER__HPP
#define PRIMALCLASSIFIER__HPP

#include "Classifier.hpp"

template < typename T >
class PrimalClassifier : public Classifier< T > {
    // Attributes
protected :
    /// Weights vector.
    std::vector<double> w;
    /// Norm used in the classification. (Euclidean Norm is the default)
    double q = 2, p = 2;

protected:
    /// Flexibility.
    double flexible = 0.0;
    /// Percentage of aproximation of the result.
    double alpha_aprox = 0.0;
public:

    double evaluate(Point< T > p) override {
        double func = 0.0;
        size_t i, dim = this->solution.w.size();

        if(p.x.size() != dim){
            std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
            return 0;
        }

        for(func = this->solution.bias, i = 0; i < dim; i++){
            func += this->solution.w[i] * p[i];
        }

        return (func >= 0)?1:-1;
    }

    /*********************************************
     *               Getters                     *
     *********************************************/

    std::string getFormulationString() override { return "Primal"; }
    /**
     * \brief GetP Return the value of the p norm.
     * \return double
     */
    double getP() const { return p; }
    /**
     * \brief GetQ Return the value of the q norm.
     * \return double
     */
    double getQ() const { return q; }

    /*********************************************
     *               Setters                     *
     *********************************************/

    /**
     * \brief setqNorm Set the q norm used by the classifier. (Euclidean norm is the default)
     * \param q Norm that will be used by the classifier.
     */
    void setqNorm(double q) {this->q = q;}
    /**
     * \brief setpNorm Set the p norm used by the classifier. (Euclidean norm is the default)
     * \param p Norm that will be used by the classifier.
     */
    void setpNorm(double p) {this->p = p;}
    /**
     * \brief Set flexibity of the classifier.
     * \param flexible flexibility.
     */
    void setFlexible(double flexible) {this->flexible = flexible;}
    /**
     * \brief Set the percentage of the aproximation.
     * \param alpha_aprox Aproximation.
     */
    void setAlphaAprox(double alpha_aprox) {this->alpha_aprox = alpha_aprox;}
};

#endif
