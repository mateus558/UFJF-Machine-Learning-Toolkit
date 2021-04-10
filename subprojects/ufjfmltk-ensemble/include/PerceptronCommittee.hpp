#ifndef PERCEPTRONCOMMITTEE_HPP_INCLUDED
#define PERCEPTRONCOMMITTEE_HPP_INCLUDED

#include "Ensemble.hpp"
#include <ufjfmltk/classifier/PrimalClassifier.hpp>
#include <ufjfmltk/classifier/Perceptron.hpp>

namespace mltk{
    namespace ensemble {
        template<typename T>
        class PerceptronCommittee : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t n = 0;
            double epslon = -1;
        public:
            explicit PerceptronCommittee(Data<T> &samples, size_t size = 10, double epslon = -1) : n(size),
                                                                                                   epslon(epslon) {
                this->samples = std::make_shared<Data<T> >(samples);
            }

            bool train() override {
                this->m_learners.resize(n);
                for (size_t i = 0; i < n; i++) {
                    this->m_learners[i] = std::make_shared<classifier::BalancedPerceptron<T>>();
                    this->m_learners[i]->setSamples(this->samples);
                    this->m_learners[i]->train();
                }
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto _classes = this->samples->classes();
                mltk::Point<double> votes(_classes.size(), 0.0);

                #if DEBUG == 1
                #pragma omp parallel for
                #endif
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    auto pred = this->m_learners[i]->evaluate(p);

                    // get prediction position
                    size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a) {
                        return (a == pred);
                    }) - _classes.begin();
                    // count prediction as a vote
                    votes[pred_pos] += 1;
                }
                size_t max_votes = std::max_element(votes.X().begin(), votes.X().end()) - votes.X().begin();
                return _classes[max_votes];
            }

            std::string getFormulationString() override {
                return "Primal";
            }
        };
    }

}

#endif