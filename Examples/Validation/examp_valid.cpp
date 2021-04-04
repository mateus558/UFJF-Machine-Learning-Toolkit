#include "Data.hpp"
#include "OneVsAll.hpp"
#include "Perceptron.hpp"

template < class T > using DataPointer = std::shared_ptr<mltk::Data< T > >;

int main(int argc, char *argv[]){
    mltk::DataPointer<double> sample = std::make_shared<mltk::Data<double> >();
    size_t fold = 10;
    int verbose = 2;
    mltk::classifier::Classifier< double >  *classifier = new mltk::classifier::PerceptronPrimal<double >();
    std::vector<double> error_arr(fold);

    std::vector<mltk::DataPointer<double> > folds(fold);
    sample->load("iris.data");
    double sizes = sample->size() / fold;
    std::cout << *sample << std::endl;

    auto classes = sample->getClasses();
    auto classDistribution = sample->getClassesDistribution();
    std::vector<mltk::DataPointer<double> > points_by_class(classes.size());
    std::vector<size_t> new_class_distribution(classes.size());
    auto train_sample = std::make_shared<mltk::Data<double> >();
    auto test_sample = std::make_shared<mltk::Data<double> >();

    for(size_t i = 0; i < classes.size(); i++){
        points_by_class[i] = std::make_shared<mltk::Data<double> >();
    }
    for(size_t i = 0; i < fold; i++){
        folds[i] = std::make_shared<mltk::Data<double> >();
    }

    for(size_t i = 0; i < classes.size(); i++){
        std::cout << "Class: " << classes[i] << std::endl;
        std::cout << "Class Distribution: " << classDistribution[i] << std::endl;
    }
    
    for(auto it = sample->begin(); it != sample->end(); it++){
        auto point = (*it);
        if(classes.size() > 2){
            points_by_class[point->Y()-1]->insertPoint(point);
        }else{
            if(point->Y() == -1){
                points_by_class[1]->insertPoint(point);
            }else{ 
                points_by_class[0]->insertPoint(point);
            }
        }
    }
    
    for(size_t i = 0; i < classes.size(); i++){
        new_class_distribution[i] = ((double)classDistribution[i]/ sample->size()) * sizes;
    }

    std::vector<size_t> markers(classes.size(), 0);
    for(size_t i = 0; i < fold; i++){
        for(size_t j = 0; j < points_by_class.size(); j++){
            for(size_t k = markers[j], l = 0; markers[j] < points_by_class[j]->size() && l < new_class_distribution[j]; k++, l++){
                if(i != fold-1){
                    train_sample->insertPoint((*points_by_class[j])[k]);
                }else{ 
                    test_sample->insertPoint((*points_by_class[j])[k]);
                }
                markers[j]++;
            }
        }
    }
    points_by_class.clear();
    new_class_distribution.clear();
    train_sample->shuffle();
    test_sample->shuffle();
    std::cout << *train_sample << std::endl;
    std::cout << *test_sample << std::endl;
}
