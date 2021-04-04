//
// Created by mateuscmarim on 05/02/2021.
//

#ifndef UFJF_MLTK_KNNENSEMBLEW_HPP
#define UFJF_MLTK_KNNENSEMBLEW_HPP

#include <Sampling.hpp>
#include "Classifier.hpp"
#include "Ensemble.hpp"
#include "KNNClassifier.hpp"
#include "../../Validation/Validation.hpp"

namespace mltk {
    namespace ensemble {
        template<typename T>
        class kNNEnsembleBaggingWRSM: public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t k;
            double r;
            std::string voting_type = "soft";
            mltk::Point<double> weights;
            std::vector<std::vector<size_t>> subspaces;

        public:
            kNNEnsembleBaggingWRSM() = default;
            kNNEnsembleBaggingWRSM(Data<T> &samples, size_t _k, double _r): k(_k), r(_r) {
                this->samples = make_data<T>(samples);
                this->m_learners.resize(7);
                this->m_learners[0] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Euclidean<T>>>(k);
                this->m_learners[1] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Lorentzian<T>>>(k);
                this->m_learners[2] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Cosine<T>>>(k);
                this->m_learners[3] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Bhattacharyya<T>>>(k);
                this->m_learners[4] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Pearson<T>>>(k);
                this->m_learners[5] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::KullbackLeibler<T>>>(k);
                this->m_learners[6] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Hassanat<T>>>(k);

                RSM<double> rsm(r, this->samples->dim(), this->seed);
                size_t samp_size = this->samples->size() / this->m_learners.size();

                std::vector<double> w;
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    auto feats = rsm(*this->samples);
                    subspaces.push_back(feats);
                    auto data = this->samples->selectFeatures(feats).sampling(samp_size, true, this->seed);
                    this->m_learners[i]->setSamples(data);
                    this->m_learners[i]->train();
                    auto classifier = dynamic_cast<classifier::Classifier<T> *>(this->m_learners[i].get());
                    auto acc = validation::kkfold(data, *classifier, 10, 10, this->seed, 0).accuracy/100.0;
                    w.push_back(acc);
                }
                this->weights = w;
            }

            bool train() override{

                return true;
            }

            double maxAccuracy(){
                std::vector<int> ids(this->samples->size(), 0);

                int i = 0;
                for(auto point: this->samples->getPoints()) {
                    for (auto& learner: this->m_learners) {
                        if(learner->evaluate(*point) == point->Y()){
                            ids[i] = 1;
                            break;
                        }
                    }
                    i++;
                }
                double sum = std::accumulate(ids.begin(), ids.end(), 0.0);
                return sum/ this->samples->size();
            }

            void setWeights(const std::vector<double> weights) {
                assert(weights.size() == this->m_learners.size());
                this->weights.X().resize(weights.size());
                this->weights = weights;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto _classes = this->samples->getClasses();
                mltk::Point<double> votes(_classes.size(), 0.0);

                if (voting_type == "soft") {
                    assert(this->weights.size() > 0);
                } else {
                    this->weights = 1;
                }
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    auto pred = this->m_learners[i]->evaluate(p.selectFeatures(subspaces[i]));
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

            std::string getFormulationString() override {
                return this->m_learners[0]->getFormulationString();
            }
        };
    }
}

#endif //UFJF_MLTK_KNNENSEMBLEW_HPP
