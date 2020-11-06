//
// Created by mateus558 on 30/03/2020.
//

#ifndef UFJF_MLTK_ONEVSALL_HPP
#define UFJF_MLTK_ONEVSALL_HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "Sampling.hpp"

/**
 * \brief Wrapper for the implementation of the one vs all multi class classification algorithm.
 */
template< typename T, template <typename > class ClassifierT>
class OneVsAll: public PrimalClassifier< T >, public DualClassifier< T > {
private:
    using ClassifierPointer = std::shared_ptr<ClassifierT< T > >;

    /// Over sampling method (optional)
    OverSampling< T > *samp_method;
    /// Vector of binary base learners 
    std::vector<ClassifierPointer> base_learners;
    /// Classifier used to pass parameters to the base learners
    ClassifierPointer classifier;

public:
    OneVsAll(std::shared_ptr<Data< T > > samples = nullptr, ClassifierPointer classifier = nullptr, OverSampling< T > *samp_method = nullptr, int _verbose = 0);

    bool train() override;

    double evaluate(Point<T> p, bool raw_value=false) override;

    std::string getFormulationString() override;

};

template< typename T, template <typename > class ClassifierT>
OneVsAll<T, ClassifierT>::OneVsAll(std::shared_ptr<Data< T > > _samples, ClassifierPointer classifier, OverSampling< T > *samp_method, int _verbose){
    this->samples = _samples;
    this->classifier = classifier;
    this->samp_method = samp_method;

    // initialize the base learners if samples were given
    if(_samples && base_learners.size() == 0){
        base_learners.resize(_samples->getClasses().size());
        for(size_t i = 0; i < _samples->getClasses().size(); ++i) {
            // copy the paraeters of the given classifier    
            base_learners[i] = std::make_shared<ClassifierT< T > >(*classifier);
        }
    }
}

template< typename T, template <typename > class ClassifierT>
bool OneVsAll<T, ClassifierT>::train() {
    auto classes = this->samples->getClasses();
    size_t current_class = 0, j, n_classes = classes.size(), size = this->samples->getSize();
    
    // initialize learners copying the parameters of the given classifier
    if(base_learners.size() == 0){
        base_learners.resize(n_classes);
        for(size_t i = 0; i < n_classes; ++i) {
            base_learners[i] = std::make_shared<ClassifierT< T > >(*this->classifier);
        }
    } 

    // iterate over each base learner
    for(auto &learner: base_learners){
        DataPointer<T> temp_samples = std::make_shared<Data< T > >();
        
        // copy samples and set all classes not being considered to -1
        temp_samples->copy(*this->samples);
        temp_samples->setClasses({-1, 1});
        for(j = 0; j < size; j++) {
            (*temp_samples)[j]->Y() = ((*temp_samples)[j]->Y() == classes[current_class]) ? 1 : -1;
        }
        
        // if a over sampling algorithm were given, apply it
        if(samp_method){
            temp_samples->computeClassesDistribution();
            (*samp_method)(temp_samples);
        }

        // train the current learner
        learner->setSamples(temp_samples);
        learner->train();
        
        // consider the next class
        current_class++;
    }

    return true;
}

template< typename T, template <typename > class ClassifierT>
double OneVsAll<T, ClassifierT>::evaluate(Point<T> p, bool raw_value) {
    auto classes = this->samples->getClasses();
    std::vector<double> dist_hyperplanes(base_learners.size());

    // classify the point as the class with maximum distance
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