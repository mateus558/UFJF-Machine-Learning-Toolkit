//
// Created by mateuscmarim on 05/01/2021.
//

#ifndef UFJF_MLTK_AUTOWEIGHTEDVOTING_HPP
#define UFJF_MLTK_AUTOWEIGHTEDVOTING_HPP

#include "Ensemble.hpp"
#include "VotingClassifier.hpp"
#include "../../Validation/Validation.hpp"

namespace mltk {
    namespace ensemble {
        template<typename T>
        class AutoWeightedVoting : public Ensemble<T>, public classifier::Classifier<T> {
        private:
            size_t p_size = 50, fold = 5;
            double F = 0.8, CR = 0.9;
            std::vector<double> best_weights;
            VotingClassifier<T> voter;
            mltk::validation::TrainTestPair<T> valid_pair;
            std::mt19937 generator;

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

            double objective_function(Point<double> const& x, Data<T>& data){
                this->seed++;
                voter.setWeights(x.X());
                return validation::kfold(data, voter, fold, this->seed, 0);
            }

            std::vector<Point<double>> init_population(){
                std::vector<Point<double>> P;
                std::uniform_real_distribution<double> dist(0., 1.);
                generator.seed(this->seed);

                for(size_t i = 0; i < p_size; i++){
                    std::vector<double> w(this->learners.size());
                    for(double & j : w){
                        j = dist(generator);
                    }
                    P.emplace_back(w);
                    P[P.size()-1] = simplex_projection(P[P.size()-1]);
                }
                return P;
            }

            std::vector<double> eval_population(std::vector<Point<double>> const& population){
                std::vector<double> costs(population.size());
                Data<T> data;
                data.copy(*this->samples);
                //#pragma omp parallel for default(none) shared(population, costs, data)
                for(size_t i = 0; i < population.size(); i++){
                    costs[i] = objective_function(population[i], data);
                }
                return costs;
            }

            std::pair<Point<double>, double> get_best_solution(std::vector<Point<double>> const& population, std::vector<double> const& costs){
                auto best_pos = std::min_element(costs.begin(), costs.end()) - costs.begin();
                return std::make_pair(population[best_pos], costs[best_pos]);
            }

            Point<double> new_sample(Point<double> const& P0, std::vector<Point<double>> const& population){
                std::uniform_int_distribution<size_t> dist(0, population.size()-1);
                size_t pos = 0;
                generator.seed(this->seed++);

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

                std::uniform_int_distribution<size_t> distNP(0, this->learners.size()-1);
                size_t cut_point = distNP(generator);
                Point<double> S(this->learners.size());
                for(int i = 0; i < S.size(); i++){
                    if(i == cut_point || dist(generator) < CR){
                        S[i] = P3[i] + F * (P1[i]-P2[i]);
                    }else{
                        S[i] = P0[i];
                    }
                }
                return simplex_projection(S);
            }


        public:
            AutoWeightedVoting() = default;

            template<template<typename...> class WeakLearner,
                    template<typename...> class... WeakLearners>
            AutoWeightedVoting(Data<T> &samples, WeakLearner<T> flearner, WeakLearners<T>... weak_learners){
                this->samples = std::make_shared<Data<T> >(samples);
                fillLearnersVector(flearner, weak_learners...);
            }

            bool train() override {
                //voter.setSamples(this->samples);
                voter.setLearners(this->learners);
                //voter.train();
                auto population = init_population();
                auto p_costs = eval_population(population);
                auto Sbest = get_best_solution(population, p_costs);

                for(size_t i = 0; i < population.size(); i++){
                    std::clog << population[i] << ", cost: " << p_costs[i] << std::endl;
                }
                std::clog << "Best weights: " << Sbest.first << ", Cost: " << Sbest.second << std::endl;
                for(int G = 0; G < this->MAX_IT; G++){
                    std::clog << "\nGeneration " << G <<"\n" <<std::endl;
                    std::vector<Point<double> > new_population;
                    for(size_t i = 0; i < population.size(); i++){
                        auto Si = new_sample(population[i], population);
                        double si_cost = objective_function(Si, *this->samples);
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
                    Sbest = (Sbest.second > candidate.second)?candidate:Sbest;
                    std::clog << "Best weights: " << Sbest.first << ", Cost: " << Sbest.second << std::endl;
                    if(Sbest.second == 0){
                        break;
                    }
                }

                std::cout << "Best weights cost: " << objective_function(Sbest.first, *this->samples) << std::endl;
                return true;
            }

            double evaluate(const Point<T> &p, bool raw_value = false) override {
                return double();
            }

            std::string getFormulationString() override {
                return this->learners[0]->getFormulationString();
            }
        };
    }
}
#endif //UFJF_MLTK_AUTOWEIGHTEDVOTING_HPP