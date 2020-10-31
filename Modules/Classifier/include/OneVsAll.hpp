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
    using ClassifierPointer = std::shared_ptr<ClassifierT< T > >;

    std::vector<ClassifierPointer> base_learners;
    ClassifierPointer classifier;

public:
    OneVsAll(std::shared_ptr<Data< T > > samples = nullptr, ClassifierPointer classifier = nullptr, int _verbose = 0);

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;

    std::string getFormulationString() override;

};

template< typename T, template <typename > class ClassifierT>
OneVsAll<T, ClassifierT>::OneVsAll(std::shared_ptr<Data< T > > _samples, ClassifierPointer classifier, int _verbose) {
    this->samples = _samples;
    this->verbose = _verbose;
    this->classifier = classifier;
    
    if(_samples && base_learners.size() == 0){
        base_learners.resize(_samples->getClasses().size());
        for(size_t i = 0; i < _samples->getClasses().size(); ++i) {
            base_learners[i] = std::make_shared<ClassifierT< T > >(*classifier);
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
            base_learners[i] = std::make_shared<ClassifierT< T > >(*this->classifier);
        }
    } 

    for(auto &learner: base_learners){
        Data<T> temp_samples;
        
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
double OneVsAll<T, ClassifierT>::evaluate(Point<T> p, bool raw_value) {
    auto classes = this->samples->getClasses();
    std::vector<double> dist_hyperplanes(base_learners.size());

    std::transform(base_learners.begin(), base_learners.end(), dist_hyperplanes.begin(), [&p](auto &learner){
        return learner->evaluate(p, true);
    });
    
    size_t max_index = std::max_element(dist_hyperplanes.begin(), dist_hyperplanes.end()) - dist_hyperplanes.begin();

    return classes[max_index];
}

template< typename T, template <typename > class ClassifierT>
std::string OneVsAll<T, ClassifierT>::getFormulationString() {
    return ClassifierT< T >().getFormulationString();
}


#endif //UFJF_MLTK_ONEVSALL_HPP