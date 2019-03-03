/*! Incremental Margin Algorithm implementations
   \file IMA.hpp
   \author Mateus Coutinho Marim
*/

//
// Created by mateus558 on 01/08/17.
//

#ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
#define CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP

#include <vector>
#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal.
 */
template < typename T >
class IMAp : public PrimalClassifier< T > {
private:
    double margin;
    /// Support vector.
    std::vector<int> svs;

public:
    explicit IMAp(std::shared_ptr<Data< T > > samples = nullptr, double margin = 0.0, Solution *initial_solution = nullptr);

    bool train() override;
    double evaluate(Point< T >  p) override;
    /**
     * \brief Get the indexes of the support vectors.
     * \return std::vector<int> 
     **/
    std::vector<int> getSupportVectors(){ return svs; }
};

/**
 * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal with fixed margin.
 */
template < typename T >
class IMApFixedMargin : public PrimalClassifier< T >  {
private:
    int n, maiorn = 0, flagNao1aDim;
    double margin = 0.0, maiorw = 0.0;
    unsigned long tMax;

    /// Support vector.
    std::vector<int> svs;

public:
    explicit IMApFixedMargin(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 0, Solution *initial_solution = nullptr);

    bool train() override;
    double evaluate(Point< T >  p) override;
    inline int* getFlagNot1aDim() {return &flagNao1aDim; }
    inline unsigned long* gettMax(){ return &tMax; }
};

template < typename T >
class IMADual : public DualClassifier< T >  {
private:
    double margin = 0;
    /// Support vector.
    std::vector<int> svs;
public:
    explicit IMADual(std::shared_ptr<Data< T > > samples = nullptr, Kernel *k = nullptr, double rate = 1, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point< T >  p) override;
    /**
     * \brief Get the indexes of support vectors.
     * \return std::vector<int> 
     **/
    std::vector<int> getSupportVectors(){ return svs; }
};

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
