//
// Created by mateuscmarim on 19/11/2020.
//

#ifndef UFJF_MLTK_BAGGINGCLASSIFIER_HPP
#define UFJF_MLTK_BAGGINGCLASSIFIER_HPP

#include "Ensemble.hpp"

namespace mltk {
    namespace ensemble {
        template<typename T>
        class BaggingClassifier : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t n_estimators = 10;
            size_t seed = 0;

        public:
            BaggingClassifier() = default;

            template<class Estimator>
            explicit BaggingClassifier(const Data<T> &samples, const Estimator &estimator, size_t n_estimators = 10,
                                       size_t seed = 0)
                    : n_estimators(n_estimators), seed(seed) {
                this->samples = mltk::make_data<T>(samples);
                this->m_learners.resize(n_estimators);
                for (size_t i = 0; i < n_estimators; i++) {
                    this->m_learners[i] = std::make_shared<Estimator>(estimator);
                }
            }

            bool train() override {
                size_t samp_size = this->samples->size() / n_estimators;
                for (size_t i = 0; i < n_estimators; i++) {
                    this->m_learners[i]->setSeed(this->seed+i);
                    this->m_learners[i]->setSamples(this->samples->sampling(samp_size, true, seed+i));
                    this->m_learners[i]->train();
                }
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto classes = this->samples->classes();
                Point<int> votes(classes.size());
                for (size_t i = 0; i < n_estimators; i++) {
                    int pred = this->m_learners[i]->evaluate(p);
                    size_t pred_pos = std::find(classes.begin(), classes.end(), pred) - classes.begin();
                    votes[pred_pos]++;
                }
                return classes[std::max_element(votes.X().begin(), votes.X().end()) - votes.X().begin()];
            }

            std::string getFormulationString() override {
                return this->m_learners[0]->getFormulationString();
            }
        };
    }
}

#endif //UFJF_MLTK_BAGGINGCLASSIFIER_HPP
