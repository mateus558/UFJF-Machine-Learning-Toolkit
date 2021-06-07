#ifndef VOTING_CLASSIFIER_HPP_DEFINED
#define VOTING_CLASSIFIER_HPP_DEFINED

#include "Ensemble.hpp"
#include <ufjfmltk/classifier/Classifier.hpp>
#include <memory>
#include <vector>
#include <string>
#include <ufjfmltk/core/Learner.hpp>

namespace mltk{
    namespace ensemble {
        template<typename T>
        class VotingClassifier : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            mltk::Point<double> weights;
            std::string voting_type;

        public:
            VotingClassifier() = default;

            template<class... Learners>
            VotingClassifier(Data<T> &samples, const std::string &voting_type,
                Learners... weak_learners): voting_type(voting_type) {
                this->samples = std::make_shared<Data<T> >(samples);
                this->m_learners = {std::make_shared<Learners>(std::forward<Learners>(weak_learners))...};
            }

            bool train() override {
                // train each one of the given weak learners
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    this->m_learners[i]->setSeed(this->seed);
                    this->m_learners[i]->setSamples(this->samples);
                    this->m_learners[i]->train();
                }
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto _classes = this->samples->classes();
                mltk::Point<double> votes(_classes.size(), 0.0);

                if (voting_type == "soft") {
                    assert(this->weights.size() > 0);
                } else {
                    this->weights = Point<double>(this->m_learners.size(), 1);
                }

                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    auto pred = this->m_learners[i]->evaluate(p);
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
                assert(weights.size() == this->m_learners.size());
                this->weights.X().resize(weights.size());
                this->weights = weights;
            }

            Point<double> getWeights(){ return this->weights; }

            void setVotingType(const std::string& vote_type){ this->voting_type = vote_type; }

            void setLearners(std::vector<LearnerPointer<T>>& m_learners){
                this->m_learners = m_learners;
            }

            std::string getFormulationString() override {
                return "Primal";
            }

            VotingClassifier<T>& operator=(VotingClassifier<T> const& voter){
                this->m_learners = voter.m_learners;
                this->weights = voter.weights;
                this->samples = voter.samples;
            }
        };
    }
}

#endif