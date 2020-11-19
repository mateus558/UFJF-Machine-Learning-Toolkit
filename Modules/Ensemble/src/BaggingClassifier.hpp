//
// Created by mateuscmarim on 19/11/2020.
//

#ifndef UFJF_MLTK_BAGGINGCLASSIFIER_HPP
#define UFJF_MLTK_BAGGINGCLASSIFIER_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"

namespace mltk {

    template<typename T>
    class BaggingClassifier : public Ensemble<T>, public Classifier<T> {
    private:
        size_t n_estimators = 10;
        size_t seed = 0;

    public:
        BaggingClassifier()=default;

        template< template<typename > class Estimator >
        explicit BaggingClassifier(const Data< T > &samples, const Estimator<T> &estimator, size_t n_estimators = 10, size_t seed = 0)
        : n_estimators(n_estimators), seed(seed)
        {
            this->samples = mltk::make_data<T>(samples);
            this->learners.resize(n_estimators);
            for(size_t i = 0; i < n_estimators; i++){
                this->learners[i] = std::make_shared< Estimator< T > >(estimator);
            }
        }

        bool train() override {
            size_t samp_size = this->samples->getSize()/n_estimators;
            for(size_t i = 0; i < n_estimators; i++){
                this->learners[i]->setSamples(this->samples->sampling(samp_size, true, seed));
                this->learners[i]->train();
            }
            return true;
        }

        double evaluate(const Point< T > &p, bool raw_value=false) override {
            auto classes = this->samples->getClasses();
            Point<int> votes(classes.size());
            for(size_t i = 0; i < n_estimators; i++){
                int pred = this->learners[i]->evaluate(p);
                size_t pred_pos = std::find(classes.begin(), classes.end(), pred) - classes.begin();
                votes[pred_pos]++;
            }
            return classes[std::max_element(votes.X().begin(), votes.X().end())-votes.X().begin()];
        }

        std::string getFormulationString() override{
            return this->learners[0]->getFormulationString();
        }
    };

}

#endif //UFJF_MLTK_BAGGINGCLASSIFIER_HPP
