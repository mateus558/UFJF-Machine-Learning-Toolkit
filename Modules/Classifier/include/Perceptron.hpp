/*! Perceptron algorithm implementations
   \file Perceptron.hpp
   \author Mateus Coutinho Marim
*/

#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "../../Core/include/MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Perceptron primal algorithm.
 */
template < typename T >
class PerceptronPrimal : public PrimalClassifier< T >  {
public:
    explicit PerceptronPrimal(std::shared_ptr<Data< T > > samples = nullptr, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p, bool raw_value=false) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron primal with fixed margin algorithm.
 */
template < typename T >
class PerceptronFixedMarginPrimal : public PrimalClassifier< T >  {
public:
    explicit PerceptronFixedMarginPrimal(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T > p, bool raw_value=false) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual algorithm.
 */
template < typename T >
class PerceptronDual : public DualClassifier< T >  {
public:
    explicit PerceptronDual(std::shared_ptr<Data< T > > samples = nullptr, double rate = 0.5, int kernel_type = KernelType::INNER_PRODUCT, double kernel_param = 0, Solution *initial_solution = nullptr);
    explicit PerceptronDual(std::shared_ptr<Data< T > > samples, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual with fixed margin algorithm.
 */
template < typename T >
class PerceptronFixedMarginDual : public DualClassifier< T >  {
public:
    explicit PerceptronFixedMarginDual(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double rate = 0.5, int kernel_type = KernelType::INNER_PRODUCT, double kernel_param = 0, Solution *initial_solution = nullptr);
    explicit PerceptronFixedMarginDual(std::shared_ptr<Data< T > > samples, double gamma = 1.0, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
};

#endif
