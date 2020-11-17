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

        public:
            BalancedPerceptron(){}
            BalancedPerceptron(Data< T > &samples){
                this->samples = std::make_shared< Data< T > >(samples);
            }

            bool train() override{
                size_t epoch = 0, ite = 0;
                bool stop = false;
                double gamma1 = std::numeric_limits< double >::max(), gamma2 = std::numeric_limits< double >::max();
                
                this->weights.X().resize(this->samples->getDim(), 0.0);
                std::random_device rnd;
                std::mt19937 gen(rnd());
                std::uniform_real_distribution<> dist(0.,1.);
                std::generate(this->weights.X().begin(), this->weights.X().end(), [&gen, &dist]() { return dist(gen); });
                this->samples->shuffle();

                this->timer.Reset();

                while(epoch < this->MAX_EPOCH){ 
                    size_t errors = 0;
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

            double evaluate(const Point< T >  &p, bool raw_value=false) override {
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
        PerceptronCommittee(Data< T > &samples, size_t size = 10, double epslon = -1): n(size), epslon(epslon) {
            this->samples = std::make_shared< Data< T > >(samples);
        }
        
        bool train() override{ 
            this->learners.resize(n);
            #if DEBUG == 1
            #pragma omp parallel for
            #endif
            for(size_t i = 0; i < n; i++){
                this->learners[i] = std::make_shared<BalancedPerceptron<T>>();
                DataPointer< T > samp = mltk::make_data< T >();
                samp->copy(*this->samples);
                this->learners[i]->setSamples(samp);
                this->learners[i]->train();
            }
            return true;
        }

        double evaluate(const Point< T >  &p, bool raw_value=false) override {
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