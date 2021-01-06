//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "Sampling.hpp"

namespace mltk{
        namespace classifier {
            /**
             * \brief Wrapper for the implementation of the one vs all multi class classification algorithm.
             */
            template<typename T>
            class OneVsAll : public PrimalClassifier<T>, public DualClassifier<T> {
            private:
                using LearnerPointer = std::shared_ptr<Learner<T> >;

                /// Over sampling method (optional)
                OverSampling<T> *samp_method;
                /// Vector of binary base learners
                std::vector<LearnerPointer> base_learners;

            public:
                OneVsAll() = default;

                template<template<typename> class ClassifierType>
                OneVsAll(Data<T> &samples, ClassifierType<T> &classifier, OverSampling<T> *samp_method = nullptr,
                         int _verbose = 0) {
                    this->samples = mltk::make_data<double>(samples);
                    this->samp_method = samp_method;

                    // initialize the base learners if samples were given
                    if (this->samples && base_learners.size() == 0) {
                        base_learners.resize(this->samples->getClasses().size());
                        for (size_t i = 0; i < this->samples->getClasses().size(); ++i) {
                            // copy the parameters of the given classifier
                            base_learners[i] = std::make_shared<ClassifierType<T> >(classifier);
                        }
                    }
                }

                bool train() override;

                double evaluate(const Point<T> &p, bool raw_value = false) override;

                std::string getFormulationString() override;

            };

            template<typename T>
            bool OneVsAll<T>::train() {
                auto classes = this->samples->getClasses();
                size_t current_class = 0, j, n_classes = classes.size(), size = this->samples->getSize();

                // iterate over each base learner
                for (auto &learner: base_learners) {
                    Data<T> temp_samples;

                    // copy samples and set all classes not being considered to -1
                    temp_samples.copy(*this->samples);
                    temp_samples.setClasses({-1, 1});
                    for (j = 0; j < size; j++) {
                        temp_samples[j]->Y() = (temp_samples[j]->Y() == classes[current_class]) ? 1 : -1;
                    }

                    // if a over sampling algorithm were given, apply it
                    if (samp_method) {
                        temp_samples.computeClassesDistribution();
                        (*samp_method)(temp_samples);
                    }

                    // train the current learner
                    learner->setSamples(temp_samples);
                    learner->train();

                    // consider the next class
                    current_class++;
                }

                return true;
            }

            template<typename T>
            double OneVsAll<T>::evaluate(const Point<T> &p, bool raw_value) {
                auto classes = this->samples->getClasses();
                std::vector<double> dist_hyperplanes(base_learners.size());

                // classify the point as the class with maximum metrics
                std::transform(base_learners.begin(), base_learners.end(), dist_hyperplanes.begin(),
                               [&p](auto &learner) {
                                   return learner->evaluate(p, true);
                               });
                size_t max_index =
                        std::max_element(dist_hyperplanes.begin(), dist_hyperplanes.end()) - dist_hyperplanes.begin();

                return classes[max_index];
            }

            template<typename T>
            std::string OneVsAll<T>::getFormulationString() {
                return this->base_learners[0]->getFormulationString();
            }
        }
}

#endif //UFJF_MLTK_ONEVSALL_HPP