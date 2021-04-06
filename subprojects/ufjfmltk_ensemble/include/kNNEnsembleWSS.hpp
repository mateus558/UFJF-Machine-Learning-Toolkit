//
// Created by mateuscmarim on 05/02/2021.
//

#ifndef UFJF_MLTK_KNNENSEMBLEW_HPP
#define UFJF_MLTK_KNNENSEMBLEW_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"
#include "KNNClassifier.hpp"
#include "Validation.hpp"

namespace mltk {
    namespace ensemble {
        template<typename T>
        class kNNEnsembleWSS: public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t k;
            std::string voting_type = "soft";
            mltk::Point<double> weights;
            mltk::Point<double> sub_weights;
        public:
            kNNEnsembleWSS() = default;
            kNNEnsembleWSS(Data<T> &samples, size_t _k): k(_k) {
                this->samples = make_data<T>(samples);
                this->m_learners.resize(7);
                this->m_learners[0] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Euclidean<T>>>(k);
                this->m_learners[1] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Lorentzian<T>>>(k);
                this->m_learners[2] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Cosine<T>>>(k);
                this->m_learners[3] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Bhattacharyya<T>>>(k);
                this->m_learners[4] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Pearson<T>>>(k);
                this->m_learners[5] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::KullbackLeibler<T>>>(k);
                this->m_learners[6] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Hassanat<T>>>(k);

                std::vector<double> w;
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    this->m_learners[i]->setSamples(this->samples);
                    this->m_learners[i]->train();
                    auto classifier = dynamic_cast<classifier::Classifier<T> *>(this->m_learners[i].get());
                    auto acc = validation::kkfold(samples, *classifier, 10, 10, this->seed, 0).accuracy/100.0;
                    w.push_back(acc);
                }
                this->weights = w;
            }

            std::vector<double> linspace(double lower, double upper, size_t N){
                double h = (upper - lower) / static_cast<double>(N-1);
                std::vector<double> xs(N);
                std::vector<double>::iterator x;
                double val;
                for (x = xs.begin(), val = lower; x != xs.end(); ++x, val += h) {
                    *x = val;
                }
                return xs;
            }

            void optimize(Data<T> &samples, classifier::Classifier<T> &classifier, std::vector<double>& set, std::vector<double>& perm,
                          int _k, double total_sum, Point<double>& best, double& best_acc, int& N){
                auto sum = std::accumulate(perm.begin(), perm.end(), 0.0);

                if(sum > total_sum){
                    return;
                }

                if(_k == 0){
                    Point<double> sub_w(perm);

                   if(sum == total_sum){
                        this->sub_weights = sub_w;
                        auto acc = validation::kkfold(samples, classifier, 10, 10, this->seed, 0).accuracy;
                        if(acc > best_acc){
                            std::cout << (double(N)/2338.0)*100.0 << "% complete" << std::endl;
                            std::cout << "Old acc: " << best_acc << std::endl;

                            best_acc = acc;
                            best = perm;

                            std::cout << "Current best: " << best << "\nacc: " << best_acc  << "\nStep: "<< N << "\n\n";
                        }else if(N%50 == 0 && N > 0){
                            Point<double> w(perm);
                            std::cout << "current weights: " << w << std::endl;
                            std::cout << "current accuracy: " << acc << std::endl;
                            std::cout << "current best weights: " << best << std::endl;
                            std::cout << "current best accuracy: " << best_acc << std::endl;
                            std::cout << (double(N)/2338.0)*100.0 << "% complete\n\n" << std::endl;
                        }
                        N++;
                    }
                    perm.clear();
                    return;
                }

                for (int i = 0; i < set.size(); i++){
                    std::vector<double> p = perm;
                    p.push_back(set[i]);

                    optimize(samples, classifier,set, p, _k - 1, total_sum, best, best_acc,N);
                }
            }

            void optimizeSubWeights(Data<T> &samples, size_t step, double max_weight){
                Point<double> temp(this->m_learners.size(), 0), best(this->m_learners.size(), 0);
                this->sub_weights.resize(this->m_learners.size());
                std::vector<double> values = linspace(0, max_weight, step);
                Point<double> p(values);
                double best_acc = 0.0;
                int N = 0, _k = this->m_learners.size();
                std::vector<double> perm;

                std::cout << p << std::endl;

                optimize(samples, *this, values, perm, _k, max_weight, best, best_acc, N);
                this->sub_weights = best;
                std::cout << N <<  std::endl;
            }

            Point<double> getSubWeights(){
                return this->sub_weights;
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
                    if(this->sub_weights[i] == 0) continue;
                    auto pred = this->m_learners[i]->evaluate(p);
                    // get prediction position
                    size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a) {
                        return (a == pred);
                    }) - _classes.begin();
                    // count prediction as a vote
                    votes[pred_pos] += this->weights[i]*this->sub_weights[i];
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
