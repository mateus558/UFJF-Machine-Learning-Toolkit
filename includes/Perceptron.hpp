/*! Perceptron algorithm implementations
   \file Perceptron.hpp
   \author Mateus Coutinho Marim
*/

#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Perceptron primal algorithm.
 */
template < typename T >
class PerceptronPrimal : public PrimalClassifier< T >  {
public:
    explicit PerceptronPrimal(std::shared_ptr<Data< T > > samples = nullptr, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron primal with fixed margin algorithm.
 */
template < typename T >
class PerceptronFixedMarginPrimal : public PrimalClassifier< T >  {
public:
    explicit PerceptronFixedMarginPrimal(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual algorithm.
 */
template < typename T >
class PerceptronDual : public DualClassifier< T >  {
public:
    explicit PerceptronDual(std::shared_ptr<Data< T > > samples = nullptr, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual with fixed margin algorithm.
 */
template < typename T >
class PerceptronFixedMarginDual : public DualClassifier< T >  {
public:
    explicit PerceptronFixedMarginDual(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p) override;
};

#endif
