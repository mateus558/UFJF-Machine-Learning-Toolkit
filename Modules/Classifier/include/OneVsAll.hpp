//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "Classifier.hpp"

template< typename T, template <typename > class ClassifierT>
class OneVsAll: public Classifier< T > {
private:
    std::vector<ClassifierT< T > > base_learners;

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
}

template< typename T, template <typename > class ClassifierT>
bool OneVsAll<T, ClassifierT>::train() {
    auto classes = this->samples->getClasses();
    size_t i = 0, j, n_classes = classes.size(), size = this->samples->getSize();

    if(base_learners.size() == 0) base_learners.resize(n_classes);

    for(auto &learner: base_learners){
        Data<T> temp_samples;
        learner.setSamples(this->samples);
        learner.setMaxTime(this->max_time);
        learner.setLearningRate(this->rate);
        learner.setMaxIterations(this->MAX_IT);
        learner.setEPS(this->EPS);
        learner.setVerbose(this->verbose);

        temp_samples.copy(*this->samples);
        temp_samples.setClasses({-1, 1});
        for(j = 0; j < size; j++) {
            temp_samples[j]->y = (temp_samples[j]->y == classes[i]) ? 1 : -1;
        }
        learner.setSamples(std::make_shared<Data< T > >(temp_samples));
        learner.train();
        i++;
    }

    return true;
}

template< typename T, template <typename > class ClassifierT>
double OneVsAll<T, ClassifierT>::evaluate(Point<T> p) {
    for(size_t i = 1; i <= base_learners.size(); i++){
        if(base_learners[i-1].evaluate(p) == 1){
            return i;
        }
    }
    return 0;
}

template< typename T, template <typename > class ClassifierT>
std::string OneVsAll<T, ClassifierT>::getFormulationString() {
    if(this->samples && base_learners.size() == 0){
        this->base_learners.resize(this->samples->getClasses().size());
    }
    return this->base_learners[0].getFormulationString();
}


#endif //UFJF_MLTK_ONEVSALL_HPP
