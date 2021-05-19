/*! Perceptron algorithm implementations
   \file Perceptron.hpp
   \author Mateus Coutinho Marim
*/

#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include <chrono>

namespace mltk{
    namespace classifier {
        using namespace std::chrono;

        /**
         * \brief Wrapper for the implementation of the Perceptron primal algorithm.
         */
        template<typename T>
        class PerceptronPrimal : public PrimalClassifier<T> {
        public:
            explicit PerceptronPrimal(std::shared_ptr<Data<T> > samples = nullptr, double q = 2, double rate = 0.5,
                                      Solution *initial_solution = nullptr);
            PerceptronPrimal(const Data<T>& samples, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
            bool train() override;

            double evaluate(const Point<T> &p, bool raw_value = false) override;
        };

        /**
         * \brief Wrapper for the implementation of the Perceptron primal with fixed margin algorithm.
         */
        template<typename T>
        class PerceptronFixedMarginPrimal : public PrimalClassifier<T> {
        public:
            PerceptronFixedMarginPrimal() = default;
            explicit PerceptronFixedMarginPrimal(const mltk::Data<T>& samples, double gamma = 0,
                                                 double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);

            bool train() override;

            double evaluate(const Point<T> &p, bool raw_value = false) override;
        };

        /**
         * \brief Wrapper for the implementation of the Perceptron dual algorithm.
         */
        template<typename T>
        class PerceptronDual : public DualClassifier<T> {
        private:
            Solution *initial = nullptr;
        public:
            PerceptronDual() = default;
            PerceptronDual(const Data<T>& samples,
                            KernelType kernel_type, double kernel_param = 0,
                            double rate = 0.5, Solution *initial_solution = nullptr);

            explicit PerceptronDual(const Data<T>& samples, double rate = 0.5,
                                    Solution *initial_solution = nullptr);

            bool train() override;
        };

        /**
         * \brief Wrapper for the implementation of the Perceptron dual with fixed margin algorithm.
         */
        template<typename T>
        class PerceptronFixedMarginDual : public DualClassifier<T> {
        public:
            PerceptronFixedMarginDual() = default;
            explicit PerceptronFixedMarginDual(const mltk::Data<T>& samples, KernelType kernel_type = KernelType::INNER_PRODUCT,
                                               double kernel_param = 0, double gamma = 0, double rate = 0.5,
                                               Solution *initial_solution = nullptr);

            bool train() override;
        };

        template<typename T>
        class BalancedPerceptron : public classifier::PrimalClassifier<T> {
        private:
            Point<double> weights;
            double bias = 0;

        public:
            BalancedPerceptron() = default;

            explicit BalancedPerceptron(const Data<T> &samples, const size_t &seed = 0) {
                this->samples = mltk::make_data<T>(samples);
                this->seed = seed;
            }

            bool train() override {
                size_t epoch = 0, ite = 0;
                bool stop = false;
                double gamma1 = std::numeric_limits<double>::max(), gamma2 = std::numeric_limits<double>::max();
                size_t errors = 0;

                this->bias = 0;
                mltk::random_init<double>(this->weights, this->samples->dim(), this->seed);
                this->samples->shuffle(this->seed);
                this->timer.reset();

                while (epoch < this->MAX_EPOCH) {
                    errors = 0;
                    for (auto it = this->samples->begin(); it != this->samples->end(); ++it) {
                        auto point = *it;
                        int pred = ((mltk::dot(weights, *point) + bias) >= 0) ? 1 : -1;

                        if (point->Y() != pred) {
                            weights += this->rate * (*point) * point->Y();
                            bias += this->rate * bias;
                            ite++;
                            errors++;
                        }

                        if (ite > this->MAX_UP) {
                            stop = true;
                            break;
                        }

                    }
                    epoch++;
                    if (stop || this->timer.elapsed() > this->max_time || errors == 0) break;
                }

                for (auto it = this->samples->begin(); it != this->samples->end(); ++it) {
                    auto point = *(*it);
                    double func = point.Y() * (mltk::dot(weights, point) + bias);

                    if (point.Y() == 1 && func < gamma1) gamma1 = func;
                    if (point.Y() == -1 && func < gamma2) gamma2 = func;
                }

                double displacement = (gamma1 + gamma2) / 2;
                bias = bias - gamma2 + displacement;

                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value) override {
                if (raw_value) {
                    return mltk::dot(weights, p) + bias;
                } else {
                    return ((mltk::dot(weights, p) + bias) >= 0) ? 1 : -1;
                }
            }

            std::string getFormulationString() override { return "Primal"; }
        };
    }
}

#endif
