//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"

template< typename T, template <typename > class ClassifierT>
class OneVsAll: public PrimalClassifier< T >, public DualClassifier< T > {
private:
    std::vector<std::shared_ptr<ClassifierT< T > > > base_learners;

public:
    OneVsAll(std::shared_ptr<Data< T > > samples = nullptr, int _verbose = 0);

    bool train() override;

    double evaluate(Point<T> p) override;

    std::string getFormulationString() override;

};

template< typename T, template <typename > class ClassifierT>
OneVsAll<T, ClassifierT>::OneVsAll(std::shared_ptr<Data< T > > _samples, int _verbose) {
    this->samples = _samples;
    this->verbose = _verbose;
    if(_samples && base_learners.size() == 0){
        base_learners.resize(_samples->getClasses().size());
        for(size_t i = 0; i < _samples->getClasses().size(); ++i) {
            base_learners[i] = std::make_shared<ClassifierT< T >>();
        }
    }
}

template< typename T, template <typename > class ClassifierT>
bool OneVsAll<T, ClassifierT>::train() {
    auto classes = this->samples->getClasses();
    size_t current_class = 0, j, n_classes = classes.size(), size = this->samples->getSize();
    if(base_learners.size() == 0){
        base_learners.resize(n_classes);
        for(size_t i = 0; i < n_classes; ++i) {
            base_learners[i] = std::make_shared<ClassifierT< T >>();
        }
    } 
    for(auto &learner: base_learners){
        Data<T> temp_samples;
        learner->setSamples(this->samples);
        learner->setMaxTime(this->max_time);
        learner->setLearningRate(this->rate);
        learner->setMaxIterations(this->MAX_IT);
        learner->setEPS(this->EPS);
        learner->setVerbose(this->verbose);

        temp_samples.copy(*this->samples);
        temp_samples.setClasses({-1, 1});
        for(j = 0; j < size; j++) {
            temp_samples[j]->y = (temp_samples[j]->y == classes[current_class]) ? 1 : -1;
        }
        learner->setSamples(std::make_shared<Data< T > >(temp_samples));
        learner->train();
        current_class++;
    }

    return true;
}

template< typename T, template <typename > class ClassifierT>
double OneVsAll<T, ClassifierT>::evaluate(Point<T> p) {
    auto classes = this->samples->getClasses();
    for(size_t i = 1; i <= base_learners.size(); i++){
        if(base_learners[i-1]->evaluate(p) == 1){
            return classes[i-1];
        }
    }
    return 0;
}

template< typename T, template <typename > class ClassifierT>
std::string OneVsAll<T, ClassifierT>::getFormulationString() {
    return ClassifierT<T>().getFormulationString();
}


#endif //UFJF_MLTK_ONEVSALL_HPP
