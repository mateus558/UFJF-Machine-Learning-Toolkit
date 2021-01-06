#ifndef VOTING_CLASSIFIER_HPP_DEFINED
#define VOTING_CLASSIFIER_HPP_DEFINED

#include "Ensemble.hpp"
#include "Classifier.hpp"
#include <memory>
#include <vector>
#include <string>
#include "Learner.hpp"
namespace mltk{
    namespace ensemble {
        template<typename T>
        class VotingClassifier : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            mltk::Point<double> weights;
            std::string voting_type;

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            void fillLearnersVector(WeakLearner<T> flearner) {
                this->learners.push_back(std::make_shared<WeakLearner<T> >(flearner));
            }

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            void fillLearnersVector(WeakLearner<T> flearner, WeakLearners<T>... weak_learners) {
                this->learners.push_back(std::make_shared<WeakLearner<T> >(flearner));
                fillLearnersVector(weak_learners...);
            }

        public:
            VotingClassifier() = default;

            template<template<typename...> class WeakLearner>
            VotingClassifier(Data<T> &samples, const std::string &voting_type, WeakLearner<T> flearner)
                    : voting_type(voting_type) {
                this->samples = std::make_shared<Data<T> >(samples);
                this->learners.push_back(std::make_shared<WeakLearner<T> >(flearner));
            }

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            VotingClassifier(Data<T> &samples, const std::string &voting_type, WeakLearner<T> flearner,
                             WeakLearners<T>... weak_learners): voting_type(voting_type) {
                this->samples = std::make_shared<Data<T> >(samples);
                fillLearnersVector(flearner, weak_learners...);
            }

            VotingClassifier(Data<T> &samples, const std::string &voting_type, std::vector<LearnerPointer<T>>& _learners)
                    : voting_type(voting_type) {
                this->samples = std::make_shared<Data<T>>(samples);
                this->learners = _learners;
            }

            bool train() override {
//#pragma omp parallel for default(none)
                // train each one of the given learners
                for (size_t i = 0; i < this->learners.size(); i++) {
                    this->learners[i]->setSamples(this->samples);
                    this->learners[i]->train();
                }
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto _classes = this->samples->getClasses();
                mltk::Point<double> votes(_classes.size(), 0.0);

                if (voting_type == "soft") {
                    assert(this->weights.size() > 0);
                } else {
                    this->weights = 1;
                }

                for (size_t i = 0; i < this->learners.size(); i++) {
                    auto pred = this->learners[i]->evaluate(p);
                    // get prediction position
                    size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a) {
                        return (a == pred);
                    }) - _classes.begin();
                    // count prediction as a vote
                    votes[pred_pos] += this->weights[i];
                }

                size_t max_votes = std::max_element(votes.X().begin(), votes.X().end()) - votes.X().begin();
                return _classes[max_votes];
            }

            void setWeights(const std::vector<double> weights) {
                assert(weights.size() == this->learners.size());
                this->weights.X().resize(weights.size());
                this->weights = weights;
            }

            void setLearners(std::vector<LearnerPointer<T>>& learners){
                this->learners = learners;
            }

            std::string getFormulationString() override {
                return this->learners[0]->getFormulationString();
            }
        };
    }
}

#endif