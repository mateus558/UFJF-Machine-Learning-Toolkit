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
#include "Perceptron.hpp"

namespace mltk{
    namespace classifier {
        /**
         * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal.
         */
        template<typename T = double>
        class IMAp : public PrimalClassifier<T> {
        private:
            double margin;
            /// Support vector.
            std::vector<int> svs;

        public:
            IMAp() = default;

            explicit IMAp(const Data <T>& samples, int q = 2, double flexible = 0.0, double margin = 0.0,
                          Solution* initial_solution = nullptr);

            bool train() override;

            double evaluate(const Point <T> &p, bool raw_value = false) override;

            /**
             * \brief Get the indexes of the support vectors.
             * \return std::vector<int>
             **/
            std::vector<int> getSupportVectors() { return svs; }
        };

        /**
         * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal with fixed margin.
         */
        template<typename T = double>
        class IMApFixedMargin : public PrimalClassifier<T> {
        private:
            int n, maiorn = 0, flagNao1aDim;
            double margin = 0.0, maiorw = 0.0;
            unsigned long tMax;

            /// Support vector.
            std::vector<int> svs;

        public:
            IMApFixedMargin() = default;
            explicit IMApFixedMargin(const mltk::Data<T>& samples, double gamma = 0,
                                     Solution *initial_solution = nullptr);

            bool train() override;

            double evaluate(const Point <T> &p, bool raw_value = false) override;

            inline int *getFlagNot1aDim() { return &flagNao1aDim; }

            inline unsigned long *gettMax() { return &tMax; }
        };

        template<typename T = double>
        class IMADual : public DualClassifier<T> {
        private:
            double margin = 0;
            /// Support vector.
            std::vector<int> svs;
        public:
            IMADual() = default;
            explicit IMADual(KernelType kernel_type, double kernel_param=0,
                             double rate = 1, Solution* initial_solution = nullptr);
            explicit IMADual(const Data<T>& samples, KernelType kernel_type=KernelType::INNER_PRODUCT, double kernel_param=0,
                             double rate = 1, Solution* initial_solution = nullptr);

            bool train() override;
            /**
             * \brief Get the indexes of support vectors.
             * \return std::vector<int>
             **/
            std::vector<int> getSupportVectors() { return svs; }
        };
    }
}

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
