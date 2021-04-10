//
// Created by mateuscmarim on 20/01/2021.
//

#ifndef UFJF_MLTK_KNNENSEMBLE_HPP
#define UFJF_MLTK_KNNENSEMBLE_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"
#include "KNNClassifier.hpp"
#include "Sampling.hpp"
#include "Validation.hpp"

namespace mltk {
namespace ensemble {
        template<typename T>
        class kNNEnsembleWPDSM : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t k;
            double alpha=0.5;
            bool test_mode = false;
            std::string voting_type = "soft";
            mltk::Point<double> weights, sub_weights;
            std::vector<std::vector<size_t>> subspaces;
        public:
            kNNEnsembleWPDSM() = default;
            kNNEnsembleWPDSM(Data<T> &samples, size_t _k, double alpha, std::vector<size_t> subs_order=std::vector<size_t>()): k(_k), alpha(alpha) {
                this->samples = make_data<T>(samples);
                this->m_learners.resize(7);
                this->m_learners[0] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Euclidean<T>>>(k);
                this->m_learners[1] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Lorentzian<T>>>(k);
                this->m_learners[2] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Cosine<T>>>(k);
                this->m_learners[3] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Bhattacharyya<T>>>(k);
                this->m_learners[4] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Pearson<T>>>(k);
                this->m_learners[5] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::KullbackLeibler<T>>>(k);
                this->m_learners[6] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Hassanat<T>>>(k);

                DSM<T> dsm(*this->samples, this->m_learners.size(), this->samples->dim() * 0.5, alpha);
                std::vector<double> w;

                subspaces = dsm(*this->samples);
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    Data<T> data;
                    if(subs_order.empty()) {
                        data = this->samples->selectFeatures(subspaces[i]);
                    }else{
                        data = this->samples->selectFeatures(subspaces[subs_order[i]]);
                    }
                    this->m_learners[i]->setSamples(data);
                    this->m_learners[i]->train();
                    auto classifier = dynamic_cast<classifier::Classifier<T> *>(this->m_learners[i].get());
                    auto acc = validation::kkfold(data, *classifier, 10, 10, 42, 0).accuracy/100.0;
                    w.push_back(acc);
                }
                this->weights = w;
                this->sub_weights = w;
                this->sub_weights = 1;
                //std::cout << this->weights << std::endl;
            }

            bool train() override{
                return true;
            }

            void set_test(bool is_test){ this->test_mode = is_test; }

            void optimize(Data<T> &samples, Point<double>& set, std::vector<double>& perm,
                          int _k, double total_sum, Point<double>& best, double& best_acc, int& N){
                auto sum = std::accumulate(perm.begin(), perm.end(), 0.0);

                if(sum > total_sum){
                    return;
                }

                if(_k == 0){
                    Point<double> sub_w(perm);

                    if(sum == total_sum){
                        this->sub_weights = sub_w;
                        auto rng = std::default_random_engine{};
                        std::shuffle(subspaces.begin(), subspaces.end(), rng);
                        for (size_t i = 0; i < this->m_learners.size(); i++) {
                            auto data = this->samples->selectFeatures(subspaces[i]);
                            this->m_learners[i]->setSamples(data);
                        }
                        auto acc = validation::kkfold(samples, *this, 10, 10, this->seed, 0).accuracy;
                        if(acc > best_acc){
                            best_acc = acc;
                            best = perm;

                            std::cout <<"\t"<< N << "\t\t" << Point<double>(perm) << "\t\t" << acc << "\t\t" << best << "\t\t" << best_acc << std::endl;
                        }else if(N%50 == 0 && N > 0){
                            std::cout <<"\t"<< N << "\t\t" << Point<double>(perm) << "\t\t" << acc << "\t\t" << best << "\t\t" << best_acc << std::endl;
                        }
                        N++;
                    }
                    perm.clear();
                    return;
                }

                for (int i = 0; i < set.size(); i++){
                    std::vector<double> p = perm;
                    p.push_back(set[i]);

                    optimize(samples,set, p, _k - 1, total_sum, best, best_acc,N);
                }
            }

            void optimizeSubWeights(Data<T> &samples, size_t step, double max_weight){
                Point<double> temp(this->m_learners.size(), 0), best(this->m_learners.size(), 0);
                this->sub_weights.resize(this->m_learners.size());
                Point<double> values = mltk::linspace<T>(0, max_weight, step);
                Point<double> p(values);
                double best_acc = 0.0;
                int N = 0, _k = this->m_learners.size();
                std::vector<double> perm;

                std::cout << "----------------------------------------------------------------------------------------------------------------\n";
                std::cout << "\tstep\t\t\tweights\t\t\taccuracy\t\tbest_weights\t\tbest_acc\n";
                std::cout << "----------------------------------------------------------------------------------------------------------------\n";

                optimize(samples,values, perm, _k, max_weight, best, best_acc, N);
                this->sub_weights = best;

                std::cout << N <<  std::endl;
            }

            Point<double> getSubWeights(){
                return this->sub_weights;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                auto _classes = this->samples->classes();
                mltk::Point<double> votes(_classes.size(), 0.0);
                double res = -100;
                if (voting_type == "soft") {
                    assert(this->weights.size() > 0);
                } else {
                    this->weights = 1;
                }
                for (size_t i = 0; i < this->m_learners.size(); i++) {
                    auto pred = this->m_learners[i]->evaluate(p.selectFeatures(subspaces[i]));
                    if(pred == p.Y()){
                        res = p.Y();
                    }
                    if(!this->test_mode) {
                        // get prediction position
                        size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a) {
                            return (a == pred);
                        }) - _classes.begin();
                        // count prediction as a vote
                        votes[pred_pos] += this->weights[i] * this->sub_weights[i] *
                                           this->m_learners[i]->getPredictionProbability();
                    }
                }
                if(!test_mode){
                    size_t max_votes = std::max_element(votes.X().begin(), votes.X().end()) - votes.X().begin();
                    return _classes[max_votes];
                }
                return res;
            }

            std::string getFormulationString() override {
                return this->m_learners[0]->getFormulationString();
            }
        };
    }
}
#endif //UFJF_MLTK_KNNENSEMBLE_HPP
