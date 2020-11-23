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
            size_t n_estimators{};
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
                size_t _size = this->samples->getSize(), K = this->samples->getClasses().size();
                Point<double> err(n_estimators, 0.0);
                Point<double> alpha(n_estimators, 0.0);
                // Initialize weights to an uniform distribution
                this->weights.assign(_size, 1.0/_size);

                for(size_t m = 0; m < n_estimators; m++){
                    auto learner = this->learners[m];
                    learner->setSeed(this->seed+m);
                    learner->setSamples(this->samples);
                    learner->train();
                    // compute the probability of miss classification for each point
                    Point<double> errors(_size, 0.0);
                    for(size_t i = 0; i < _size; i++){
                        auto point = (*this->samples)[i];

                        if(point->Y() != learner->evaluate(*point)) errors[i] = weights[i];
                    }
                    // compute the estimator error as the weighted average of each point error
                    err[m] = mltk::dot(weights, errors)/weights.sum();
                    // compute alpha to be used on weight update
                    alpha[m] = ((err[m] > 0)?std::log((1.0-err[m])/err[m]):1) + std::log(K - 1);
                    weights *= mltk::exp(alpha[m]*errors);
                    // Normalize weights to form a probability distribution
                    weights /= weights.sum();
                }
                this->_alpha.X().resize(n_estimators);
                this->_alpha = alpha;
                return true;
            }

            double evaluate(const Point<T>& p, bool raw_value=false) override {
                auto classes = this->samples->getClasses();
                Point<double> prob(classes.size(), 0.0);
                for(size_t c = 0; c < classes.size(); c++) {
                    for(size_t m = 0; m < n_estimators; m++) {
                        if(this->learners[m]->evaluate(p) == classes[c]) {
                            prob[c] += this->_alpha[m];
                        }
                    }
                }
                size_t class_pos = std::max_element(prob.X().begin(), prob.X().end()) - prob.X().begin();
                return classes[class_pos];
            }

            std::string getFormulationString() override {
                return this->learners[0]->getFormulationString();
            }
        };
    }
}

#endif //UFJF_MLTK_ADABOOSTCLASSIFIER_HPP
