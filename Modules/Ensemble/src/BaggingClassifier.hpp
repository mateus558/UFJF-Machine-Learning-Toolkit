//
// Created by mateuscmarim on 19/11/2020.
//

#ifndef UFJF_MLTK_BAGGINGCLASSIFIER_HPP
#define UFJF_MLTK_BAGGINGCLASSIFIER_HPP

#include "Classifier.hpp"
#include "Ensemble.hpp"

namespace mltk {

    template<typename T>
    class BaggingClassifier : public Ensemble<T> {
    private:
        size_t n_estimators = 10;
        size_t seed = 0;

    public:
        BaggingClassifier()=default;

        template< template<typename > class Estimator >
        explicit BaggingClassifier(const Data< T > &samples, const Estimator<T> &estimator, size_t n_estimators = 10, size_t seed = 0)
        : Ensemble<T>(samples), n_estimators(n_estimators), seed(seed)
        {
            this->learners.resize(n_estimators);
            for(size_t i = 0; i < n_estimators; i++){
                this->learners[i] = std::make_shared< Estimator< T > >(estimator);
            }
        }

        bool train() override {

        }

        double evaluate(const  Point< T > &p) const override {

        }

        std::string getFormulationString(){
            return this->learners[0]->getFormulationString();
        }
    };

}

#endif //UFJF_MLTK_BAGGINGCLASSIFIER_HPP
