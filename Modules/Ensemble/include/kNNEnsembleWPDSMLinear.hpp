//
// Created by mateuscmarim on 20/01/2021.
//

#ifndef UFJF_MLTK_KNNENSEMBLEDSMLINEAR_HPP
#define UFJF_MLTK_KNNENSEMBLEDSMLINEAR_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"
#include "KNNClassifier.hpp"
#include "Sampling.hpp"
#include "../../Validation/Validation.hpp"

namespace mltk {
namespace ensemble {
        template<typename T>
        class kNNEnsembleWPDSMLinear : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t k;
            double alpha=0.5;
            std::string voting_type = "soft";
            mltk::Point<double> weights;
            std::vector<std::vector<size_t>> subspaces;
            mltk::Point<double> sub_weights;
        public:
            kNNEnsembleWPDSMLinear() = default;
            kNNEnsembleWPDSMLinear(Data<T> &samples, size_t _k): k(_k) {
                this->samples = make_data<T>(samples);
                this->learners.resize(7);
                this->learners[0] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Euclidean<T>>>(k);
                this->learners[1] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Lorentzian<T>>>(k);
                this->learners[2] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Cosine<T>>>(k);
                this->learners[3] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Bhattacharyya<T>>>(k);
                this->learners[4] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Pearson<T>>>(k);
                this->learners[5] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::KullbackLeibler<T>>>(k);
                this->learners[6] = std::make_shared<classifier::KNNClassifier<T, metrics::dist::Hassanat<T>>>(k);


                Point<double> alphas = linspace<double>(0, 0.6, 7);
                DSM<T> dsm(samples, 1, this->samples->getDim()*0.5, alphas[0]);
                std::vector<double> w;
                for (size_t i = 0; i < this->learners.size(); i++) {
                    dsm.setAlpha(alphas[i]);
                    std::vector<size_t> subspace = dsm(*this->samples)[0];
                    auto data = this->samples->selectFeatures(subspace);
                    subspaces.push_back(subspace);
                    this->learners[i]->setSamples(data);
                    this->learners[i]->train();
                    auto classifier = dynamic_cast<classifier::Classifier<T> *>(this->learners[i].get());
                    auto acc = validation::kkfold(data, *classifier, 10, 10, 42, 0).accuracy/100.0;
                    w.push_back(acc);
                }
                this->weights = w;
                this->sub_weights = 1;
                std::cout << weights << std::endl;
            }

            bool train() override{

                return true;
            }

            void optimize(Data<T> &samples, classifier::Classifier<T> &classifier, Point<double>& set, std::vector<double>& perm,
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

                    optimize(samples, classifier,set, p, _k - 1, total_sum, best, best_acc,N);
                }
            }

            void optimizeSubWeights(Data<T> &samples, size_t step, double max_weight){
                Point<double> temp(this->learners.size(), 0), best(this->learners.size(), 0);
                this->sub_weights.resize(this->learners.size());
                Point<double> values = mltk::linspace<T>(0, max_weight, step);
                Point<double> p(values);
                double best_acc = 0.0;
                int N = 0, _k = this->learners.size();
                std::vector<double> perm;
                std::cout << p << std::endl;
                std::cout << "----------------------------------------------------------------------------------------------------------------\n";
                std::cout << "\tstep\t\t\tweights\t\t\taccuracy\t\tbest_weights\t\tbest_acc\n";
                std::cout << "----------------------------------------------------------------------------------------------------------------\n";
                optimize(samples, *this, values, perm, _k, max_weight, best, best_acc, N);
                this->sub_weights = best;
                std::cout << N <<  std::endl;
            }

            Point<double> getSubWeights(){
                return this->sub_weights;
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
                    auto pred = this->learners[i]->evaluate(p.selectFeatures(subspaces[i]));
                    // get prediction position
                    size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a) {
                        return (a == pred);
                    }) - _classes.begin();
                    // count prediction as a vote
                    votes[pred_pos] += this->weights[i]*this->sub_weights[i]*this->learners[i]->getPredictionProbability();
                }
                size_t max_votes = std::max_element(votes.X().begin(), votes.X().end()) - votes.X().begin();
                return _classes[max_votes];
            }

            std::string getFormulationString() override {
                return this->learners[0]->getFormulationString();
            }
        };
    }
}
#endif //UFJF_MLTK_KNNENSEMBLE_HPP
