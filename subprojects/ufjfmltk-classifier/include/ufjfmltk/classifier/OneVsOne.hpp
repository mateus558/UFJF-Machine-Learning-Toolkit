//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSONE_HPP
#define UFJF_MLTK_ONEVSONE_HPP
#pragma once

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "ufjfmltk/core/Sampling.hpp"

namespace mltk{
    namespace classifier {
        /**
         * \brief Wrapper for the implementation of the one vs one multi class classification algorithm.
         */
        template<typename T>
        class OneVsOne : public PrimalClassifier<T>, public DualClassifier<T> {
        private:
            using LearnerPointer = std::shared_ptr<Learner<T> >;
            /// Matrix of binary base m_learners
            std::vector<std::vector<LearnerPointer> > base_learners;
            /// Over sampling method used during training (optional)
            OverSampling<T> *samp_method;

        public:
            OneVsOne() = default;

            template<template<typename> class ClassifierType>
            OneVsOne(const Data<T> &samples, ClassifierType<T> &classifier, OverSampling<T> *samp_method = nullptr,
                     int _verbose = 0) {
                this->samples = mltk::make_data<double>(samples);
                this->verbose = _verbose;
                this->samp_method = samp_method;

                auto classes = samples.classes();
                // initialize the m_learners matrix is samples were given
                if (base_learners.size() == 0) {
                    base_learners.resize(classes.size());

                    for (size_t i = 0; i < classes.size(); ++i) {
                        base_learners[i].resize(classes.size());

                        for (size_t j = 0; j < base_learners[i].size(); ++j) {
                            if (classes[i] != classes[j]) {
                                base_learners[i][j] = std::make_shared<ClassifierType<T> >(classifier);
                            }
                        }
                    }
                }
            }

            bool train() override;

            double evaluate(const Point<T> &p, bool raw_value = false) override;

            std::string getFormulationString() override;
        };

        template<typename T>
        bool OneVsOne<T>::train() {
            auto classes = this->samples->classes();
            size_t current_class = 0, j, n_classes = classes.size(), size = this->samples->size();

            for (size_t i = 0; i < n_classes; ++i) {
                for (size_t j = 0; j < n_classes; ++j) {
                    if (classes[i] != classes[j]) {
                        Data<T> temp_samples;
                        auto learner = base_learners[i][j];
                        std::vector<int> current_classes = {classes[i], classes[j]};

                        // Copy the points with the classes being trained
                        temp_samples.classesCopy(*this->samples, current_classes);
                        temp_samples.setClasses({-1, 1});
                        // Transform the classes for binary classification
                        for (size_t k = 0; k < temp_samples.size(); k++) {
                            temp_samples[k]->Y() = (temp_samples[k]->Y() == classes[i]) ? 1 : -1;
                        }

                        // If a over sampling algorithm was given, apply it to the samples
                        if (samp_method) {
                            temp_samples.computeClassesDistribution();
                            (*samp_method)(temp_samples);
                        }

                        // train the current binary learner
                        learner->setSamples(temp_samples);
                        learner->train();
                    }
                }
            }

            return true;
        }

        template<typename T>
        double OneVsOne<T>::evaluate(const Point<T> &p, bool raw_value) {
            auto classes = this->samples->classes();
            std::vector<size_t> class_votes(classes.size(), 0);

            // classify the given point as the class with maximum votes
            for (size_t i = 0; i < base_learners.size(); ++i) {
                for (size_t j = 0; j < base_learners[i].size(); ++j) {
                    if (classes[i] != classes[j]) {
                        if (base_learners[i][j]->evaluate(p) == 1) {
                            class_votes[i]++;
                        } else {
                            class_votes[j]++;
                        }
                    }
                }
            }

            auto max_index = std::max_element(class_votes.begin(), class_votes.end()) - class_votes.begin();

            return classes[max_index];
        }

        template<typename T>
        std::string OneVsOne<T>::getFormulationString() {
            return this->base_learners[0][1]->getFormulationString();
        }
    }
}

#endif //UFJF_MLTK_ONEVSONE_HPP