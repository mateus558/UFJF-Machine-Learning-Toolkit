//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"


template< typename T, template <typename > class ClassifierT>
class OneVsOne: public PrimalClassifier< T >, public DualClassifier< T > {
private:
    using ClassifierPointer = std::shared_ptr<ClassifierT< T > >;

    std::vector<std::vector<ClassifierPointer> > base_learners;
    ClassifierPointer classifier;

public:
    OneVsOne(std::shared_ptr<Data< T > > samples = nullptr, std::shared_ptr<ClassifierT< T > > classifier = nullptr, int _verbose = 0);

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;

    std::string getFormulationString() override;

};

template< typename T, template <typename > class ClassifierT>
OneVsOne<T, ClassifierT>::OneVsOne(std::shared_ptr<Data< T > > _samples, std::shared_ptr<ClassifierT< T > > classifier, int _verbose) {
    this->samples = _samples;
    this->verbose = _verbose;
    this->classifier = classifier;
    auto classes = _samples->getClasses();
    if(_samples && base_learners.size() == 0){
        base_learners.resize(_samples->getClasses().size());
        for(size_t i = 0; i < _samples->getClasses().size(); ++i) {
            base_learners[i].resize(_samples->getClasses().size());
            for(size_t j = 0; j < base_learners[i].size(); ++j) {
                if(classes[i] != classes[j]){
                    base_learners[i][j] = std::make_shared<ClassifierT< T > >(*classifier);
                }
            }
        }
    }
}

template< typename T, template <typename > class ClassifierT>
bool OneVsOne<T, ClassifierT>::train() {
    auto classes = this->samples->getClasses();
    size_t current_class = 0, j, n_classes = classes.size(), size = this->samples->getSize();
    if(base_learners.size() == 0){
        base_learners.resize(n_classes);
        for(size_t i = 0; i < n_classes; ++i) {
            base_learners[i].resize(n_classes);
            for(size_t j = 0; j < base_learners[i].size(); ++j) {
                if(classes[i] != classes[j]){
                    base_learners[i][j] = std::make_shared<ClassifierT< T > >(*this->classifier);
                }
            }
        }
    } 

    for(size_t i = 0; i < n_classes; ++i) {
        for(size_t j = 0; j < n_classes; ++j) {
            if(classes[i] != classes[j]){
                Data<T> temp_samples;
                auto learner = base_learners[i][j];
                std::vector<int> current_classes = {classes[i], classes[j]};
                
                temp_samples.classesCopy(*this->samples, current_classes);
                temp_samples.setClasses({-1, 1});
                for(size_t k = 0; k < temp_samples.getSize(); k++) {
                    temp_samples[k]->y = (temp_samples[k]->y == classes[i]) ? 1 : -1;
                }
                learner->setSamples(std::make_shared<Data< T > >(temp_samples));
                learner->train();
            }
        }
    }

    return true;
}

template< typename T, template <typename > class ClassifierT>
double OneVsOne<T, ClassifierT>::evaluate(Point<T> p, bool raw_value) {
    auto classes = this->samples->getClasses();
    std::vector<size_t> class_votes(classes.size(), 0);

    for(size_t i = 0; i < base_learners.size(); ++i) {
        for(size_t j = 0; j < base_learners[i].size(); ++j) {
            if(classes[i] != classes[j]){
                if(base_learners[i][j]->evaluate(p) == 1){
                    class_votes[i]++;
                }else{
                    class_votes[j]++;
                }
            }
        }
    }

    auto max_index = std::max_element(class_votes.begin(), class_votes.end()) - class_votes.begin();

    return classes[max_index];
}

template< typename T, template <typename > class ClassifierT>
std::string OneVsOne<T, ClassifierT>::getFormulationString() {
    return ClassifierT<T>().getFormulationString();
}


#endif //UFJF_MLTK_ONEVSALL_HPP
