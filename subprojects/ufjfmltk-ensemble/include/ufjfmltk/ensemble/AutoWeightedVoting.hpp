//
// Created by mateuscmarim on 05/01/2021.
//

#pragma once

#include "VotingClassifier.hpp"
#include <ufjfmltk/Validation.hpp>

namespace mltk {
    namespace ensemble {
        template<typename T>
        class AutoWeightedVoting : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t p_size = 1000, fold = 5;
            double F = 0.8, CR = 0.9;
            std::vector<double> best_weights;
            VotingClassifier<T> voter;
            mltk::validation::TrainTestPair<T> valid_pair;
            bool use_simplex = false;
            std::mt19937 generator;
            std::pair<Point<double>, double> Sbest;
            size_t best_generation = 0;

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            void fillLearnersVector(WeakLearner<T> flearner) {
                this->m_learners.push_back(std::make_shared<WeakLearner<T> >(flearner));
            }

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            void fillLearnersVector(WeakLearner<T> flearner, WeakLearners<T>... weak_learners) {
                this->m_learners.push_back(std::make_shared<WeakLearner<T> >(flearner));
                fillLearnersVector(weak_learners...);
            }

            Point<T> simplex_projection(Point<T> const& y) {
                Point<double> t(y.size(), 0), x(y.size()), u(y.size());
                double that = 0, tmpsum = 0, tmax = 0;
                bool bget = false;
                int i = y.size() - 2, n = y.size();

                u = y;
                std::sort(u.X().begin(), u.X().end());

                do{
                    for (size_t j = i+1; j < n; j++) {
                        t[i] += u[j] - 1;
                    }
                    if (t[i] != 0) t[i] = t[i]/double(n - i);
                    tmpsum += u[i];
                    tmax = (tmpsum-1)/double(n-i);
                    if (tmax >= u[i]) {
                        that = tmax;
                        bget = true;
                        break;
                    }
                    i--;
                }while(i >= 0);

                if (!bget) {
                    that = (tmpsum+u[n-1]-1)/n;
                }
                for(int j = 0; j < x.size(); j++){
                    x[j] = y[j] - that;
                    if(x[j] < 0) x[j] = 0;
                }

                return x;
            }

            double compute_acc(Data<T>& data, Learner<T>& learner){
                auto acc = 0;
                for(auto const& p: data){
                    auto pred = learner.evaluate(*p);
                    if(pred == p->Y()){
                        acc++;
                    }
                }
                return double(acc)/ data.size();
            }

            double objective_function(Point<double> const& x){
                double tp = 0, tn = 0, fp = 0, fn = 0;
                int n = valid_pair.test.size();
                voter.setWeights(x.X());
//                for(size_t i = 0; i < n; i++){
//                    auto point = valid_pair.test[i];
//                    auto pred = voter.evaluate(*point);
//
//                    if(pred == 1){
//                        if(point->Y() == 1){
//                            tp++;
//                        }else fp++;
//                    }else{
//                        if(point->Y() == -1){
//                            tn++;
//                        }else fn++;
//                    }
//                }
//                double div = (tp+fp)*(tp+fn)*(tn+fp)*(tn+fn);
//                double mcc = ((tp*tn)-(fp*fn))/((div > 0)?std::sqrt(div):1);
                //return 100-validation::kfold(*this->samples, voter, 10, this->seed, 0);
                return 100*compute_acc(valid_pair.test, voter);
            }

            std::vector<Point<double>> init_population(){
                std::vector<Point<double>> P;
                std::uniform_real_distribution<double> dist(0., 1.);
                //generator.seed(this->seed);

                for(size_t i = 0; i < p_size; i++){
                    std::vector<double> w(this->m_learners.size());
                    for(double & j : w){
                        j = dist(generator);
                    }
                    P.emplace_back(w);
                    if(use_simplex){
                        P[P.size()-1] = simplex_projection(P[P.size()-1]);
                    }else{
                        P[P.size()-1] = mltk::abs(P[P.size()-1]);
                    }
                }
                return P;
            }

            std::vector<double> eval_population(std::vector<Point<double>> const& population){
                std::vector<double> costs(population.size());

                for(size_t i = 0; i < population.size(); i++){
                    costs[i] = objective_function(population[i]);
                }
                return costs;
            }

            std::pair<Point<double>, double> get_best_solution(std::vector<Point<double>> const& population, std::vector<double> const& costs){
                auto best_pos = std::max_element(costs.begin(), costs.end()) - costs.begin();
                return std::make_pair(population[best_pos], costs[best_pos]);
            }

            Point<double> new_sample(Point<double> const& P0, std::vector<Point<double>> const& population){
                std::uniform_int_distribution<size_t> dist(0, population.size()-1);
                size_t pos = 0;
                //generator.seed(this->seed++);

                // parents selection
                Point<double> P1;
                do{
                    pos = dist(generator);
                    P1 = population[pos];
                }while(P1 == P0);
                Point<double> P2;
                do{
                    pos = dist(generator);
                    P2 = population[pos];
                }while(P2 == P0 || P2 == P1);

                Point<double> P3;
                do{
                    pos = dist(generator);
                    P3 = population[pos];
                }while(P3 == P0 || P3 == P1 || P3 == P2);

                std::uniform_real_distribution<double> distCR(0., 1.);
                std::uniform_int_distribution<size_t> distNP(0, this->m_learners.size() - 1);
                size_t cut_point = distNP(generator);
                Point<double> S(this->m_learners.size());
                for(int i = 0; i < S.size(); i++){
                    double _cr = distCR(generator);

                    if(i == cut_point || _cr < CR){
                        S[i] = P3[i] + F * (P1[i]-P2[i]);
                    }else{
                        S[i] = P0[i];
                    }
                }
                if(use_simplex){
                    S = mltk::abs(S);
                    S = simplex_projection(S);
                }else{
                    S = mltk::abs(S/S.norm());
                }
                return S;
            }


        public:
            AutoWeightedVoting() = default;

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            AutoWeightedVoting(Data<T> &samples, const bool simplex, WeakLearner<T> flearner, WeakLearners<T>... weak_learners){
                this->samples = std::make_shared<Data<T> >(samples);
                this->use_simplex = simplex;
                fillLearnersVector(flearner, weak_learners...);
            }

            bool train() override {
                voter.setLearners(this->m_learners);
                voter.setVotingType("soft");
                valid_pair = validation::partTrainTest(*this->samples, 10, this->seed);
                voter.setSamples(valid_pair.train);
                voter.train();
                generator.seed(this->seed);

                auto population = init_population();
                auto p_costs = eval_population(population);
                Sbest = get_best_solution(population, p_costs);

//                for(size_t i = 0; i < population.size(); i++){
//                    std::clog << population[i] << ", cost: " << p_costs[i] << std::endl;
//                }
//                std::clog << "Best weights: " << Sbest.first << ", Cost: " << Sbest.second << std::endl;
                for(int G = 0; G < this->MAX_IT; G++){
                    std::clog << "\nGeneration " << G <<std::endl;
                    std::vector<Point<double> > new_population;
                    for(size_t i = 0; i < population.size(); i++){
                        auto Si = new_sample(Sbest.first, population);
                        double si_cost = objective_function(Si);
                        if(si_cost <= p_costs[i]){
                            new_population.push_back(Si);
                        }else{
                            new_population.push_back(population[i]);
                        }
                    }
                    population = new_population;
                    p_costs = eval_population(population);
                    for(size_t i = 0; i < population.size(); i++){
                        std::clog << population[i] << ", cost: " << p_costs[i] << std::endl;
                    }
                    auto candidate = get_best_solution(population, p_costs);
                    if(Sbest.second < candidate.second){
                        Sbest = candidate;
                        best_generation = G;
                    }
                   // std::clog << "Best weights: " << Sbest.first << ", Cost: " << Sbest.second << std::endl;
                   //std::cout << Sbest.second <<std::endl;
                    if(Sbest.second == 1){
                        break;
                    }
                }
                best_weights = Sbest.first.X();
                voter.setWeights(best_weights);
                std::cout << Sbest.first << " Best weights cost: " << objective_function(Sbest.first) << std::endl;
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                voter.setWeights(best_weights);
                return voter.evaluate(p, raw_value);
            }

            std::string getFormulationString() override {
                return this->m_learners[0]->getFormulationString();
            }

            Point<double> getBestWeights(){
                return Point<double>(best_weights);
            }

            size_t getBestGeneration() { return best_generation; }

            VotingClassifier<double> getVoter(){ return voter; }
            auto getValidPair(){ return valid_pair; }
        };
    }
}
#endif //UFJF_MLTK_AUTOWEIGHTEDVOTING_HPP