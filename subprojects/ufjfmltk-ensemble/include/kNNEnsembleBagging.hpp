//
// Created by mateuscmarim on 20/01/2021.
//

#ifndef UFJF_MLTK_KNNENSEMBLE_HPP
#define UFJF_MLTK_KNNENSEMBLE_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"
#include "KNNClassifier.hpp"

namespace mltk {
namespace ensemble {
        template<typename T>
        class kNNEnsembleBagging : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t k;
        public:
            kNNEnsembleBagging() = default;
            kNNEnsembleBagging(const Data<T> &samples, size_t _k): k(_k) {
                this->samples = make_data<T>(samples);
                this->m_learners.resize(7);
                this->m_learners[0] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Euclidean<T>>>(k);
                this->m_learners[1] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Lorentzian<T>>>(k);
                this->m_learners[2] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Cosine<T>>>(k);
                this->m_learners[3] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Bhattacharyya<T>>>(k);
                this->m_learners[4] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Pearson<T>>>(k);
                this->m_learners[5] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::KullbackLeibler<T>>>(k);
                this->m_learners[6] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Hassanat<T>>>(k);

                size_t samp_size = this->samples->size() / this->m_learners.size();
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    this->m_learners[i]->setSamples(this->samples->sampling(samp_size, true, this->seed));
                    this->m_learners[i]->train();
                }
            }

            bool train() override{

                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto classes = this->samples->getClasses();
                Point<int> votes(classes.size());
                for (size_t i = 0; i < this->m_learners.size(); i++) {
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
#endif //UFJF_MLTK_KNNENSEMBLE_HPP
