#ifndef PERCEPTRONCOMMITTEE_HPP_INCLUDED
#define PERCEPTRONCOMMITTEE_HPP_INCLUDED

#include "Ensemble.hpp"
#include "PrimalClassifier.hpp"
#include "Perceptron.hpp"

namespace mltk{

template < typename T > 
    class BalancedPerceptron : public PrimalClassifier< T > {
        private:
            Point<double> weights;
            double bias = 0;
            size_t seed = 0;

        public:
            BalancedPerceptron()= default;
            explicit BalancedPerceptron(const Data< T > &samples, const size_t &seed = 0) : seed(seed){
                this->samples = std::make_shared< Data< T > >(samples);
            }

            bool train() override{
                size_t epoch = 0, ite = 0;
                bool stop = false;
                double gamma1 = std::numeric_limits< double >::max(), gamma2 = std::numeric_limits< double >::max();
                size_t errors = 0;

                mltk::random_init<double>(this->weights, this->samples->getDim(), 0);
                this->samples->shuffle();

                this->timer.Reset();

                while(epoch < this->MAX_EPOCH){ 
                    errors = 0;
                    for(auto it = this->samples->begin(); it != this->samples->end(); ++it){
                        auto point = *it;
                        int pred = ((mltk::dot(weights, *point) + bias) >= 0)?1:-1;
                        
                        if(point->Y() != pred){
                            weights += this->rate*(*point)*point->Y();
                            bias += this->rate * bias;
                            ite++;
                            errors++;
                        }
                        
                        if(ite > this->MAX_UP){ 
                            stop = true;
                            break;
                        }

                    }
                    epoch++;
                    if(stop || this->timer.Elapsed() > this->max_time || errors == 0) break;
                }

                for(auto it = this->samples->begin(); it != this->samples->end(); ++it){
                    auto point = *(*it);
                    double func = point.Y()*(mltk::dot(weights, point) + bias);
                    
                    if(point.Y() == 1 && func < gamma1) gamma1 = func;
                    if(point.Y() == -1 && func < gamma2) gamma2 = func;
                }

                double displacement = (gamma1 + gamma2)/2;
                bias = bias -  gamma2 + displacement;

                return true;
            }

            double evaluate(const Point< T >  &p, bool raw_value) override {
                if(raw_value){
                    return mltk::dot(weights, p) + bias;
                }else{
                    return ((mltk::dot(weights, p) + bias) >= 0)?1:-1;
                }
            }

            std::string getFormulationString() override { return "Primal"; }
    };

template < typename T >
class PerceptronCommittee: public Ensemble< T >, public Classifier< T > {
    private:
        size_t n = 0;
        double epslon = -1;
    public:
        explicit PerceptronCommittee(Data< T > &samples, size_t size = 10, double epslon = -1): n(size), epslon(epslon) {
            this->samples = std::make_shared< Data< T > >(samples);
        }
        
        bool train() override{ 
            this->learners.resize(n);
            for(size_t i = 0; i < n; i++){
                this->learners[i] = std::make_shared<BalancedPerceptron<T>>();
                this->learners[i]->setSamples(this->samples);
                this->learners[i]->train();
            }
            return true;
        }

        double evaluate(const Point< T >  &p, bool raw_value) override {
            auto _classes = this->samples->getClasses();
            mltk::Point<double> votes(_classes.size(), 0.0);
            
            #if DEBUG == 1
            #pragma omp parallel for
            #endif
            for(size_t i = 0; i < this->learners.size(); i++){
                auto pred = this->learners[i]->evaluate(p);

                // get prediction position
                size_t pred_pos = std::find_if(_classes.begin(), _classes.end(), [&pred](const auto &a){
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

#endif