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
            BalancedPerceptron(Data< T > &samples){
                this->samples = std::make_shared< Data< T > >(samples);
            }

            bool train() override{
                size_t epoch = 0, ite = 0;
                bool stop = false;
                double gamma1 = std::numeric_limits< double >::max(), gamma2 = std::numeric_limits< double >::max();
                this->weights.X().resize(this->samples->getDim(), 0.0);
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
                
                    if(stop || this->timer.Elapsed() > 0.5 || errors == 0) break;
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
        
    public:
        PerceptronCommittee() {}
        
        bool train() override{ 

        }

        double evaluate(const Point< T >  &p, bool raw_value=false) override {

        }

};

}

#endif