//
// Created by mateuscmarim on 20/11/2020.
//

#ifndef UFJF_MLTK_ADABOOSTCLASSIFIER_HPP
#define UFJF_MLTK_ADABOOSTCLASSIFIER_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"

namespace mltk {
    namespace ensemble {
        template<typename T>
        class AdaBoostClassifier : public Ensemble<T>, public Classifier<T> {
        private:
            size_t n_estimators;
            Point<double> weights;
            Point<double> _alpha;
        public:
            AdaBoostClassifier()=default;

            template<template<typename > class Estimator>
            explicit AdaBoostClassifier(const Data<T> &data, Estimator<T> base_estimator, const size_t n_estimators): n_estimators(n_estimators) {
                this->samples = mltk::make_data<T>(data);
                this->learners.resize(n_estimators);

                for(size_t i = 0; i < n_estimators; i++){
                    this->learners[i] = std::make_shared< Estimator<T> >(base_estimator);
                }
            }

            bool train() override {
                size_t _size = this->samples->getSize();
                Point<double> err(n_estimators, 0.0);
                Point<double> alpha(n_estimators, 0.0);
                this->weights.X().resize(_size, 1.0/_size);
                for(size_t m = 0; m < n_estimators; m++){
                    auto learner = this->learners[m];
                    learner->setSeed(this->seed+m);
                    learner->setSamples(this->samples);
                    learner->train();

                    Point<double> errors(_size, 0.0);
                    Point<double> predictions(_size, 1), y(_size, 1);
                    for(size_t i = 0; i < _size; i++){
                        auto point = (*this->samples)[i];
                        y[i] = point->Y();
                        predictions[i] = double(learner->evaluate(*point));

                        if(y[i] != predictions[i]) errors[i] = weights[i];
                    }
                    err[m] = ((weights*errors)/weights.sum()).sum();
                    alpha[m] = (err[m] > 0)?std::log(std::abs(1.0-err[m])/err[m]):0;
                    weights *= mltk::exp(alpha[m]*errors);
                    // Normalize weights to one
                    weights /= weights.sum();
                }
                this->_alpha.X().resize(n_estimators);
                this->_alpha = alpha;
                return true;
            }

            double evaluate(const Point<T>& p, bool raw_value=false) override {
                double hip = 0.0;
                for(size_t m = 0; m < n_estimators; m++){
                    hip += this->_alpha[m] * this->learners[m]->evaluate(p, true);
                }
                if(raw_value) return hip;
                else return (hip > 0)?1:-1;
            }

            std::string getFormulationString() override {
                return this->learners[0]->getFormulationString();
            }
        };
    }
}

#endif //UFJF_MLTK_ADABOOSTCLASSIFIER_HPP
