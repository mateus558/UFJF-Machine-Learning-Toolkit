#include <memory>

#include "DualClassifier.hpp"
#include "Validation.hpp"
#include "Data.hpp"
#include "Utils.hpp"
#include "Point.hpp"
#include "Random.hpp"
#include "Kernel.hpp"

using namespace std;

template < typename T >
Validation< T > ::Validation(std::shared_ptr<Data< T > > sample, Classifier< T >  *classifier, unsigned int seed){
    this->sample = sample;
    this->classifier = classifier;
    train_sample = std::make_shared<Data< T > >();
    test_sample = std::make_shared<Data< T > >();

    Random::init(seed);
    seed = Random::getSeed();
}

template < typename T >
void Validation< T > ::partTrainTest(int fold){
    double sizes = sample->getSize()/fold;
    std::vector<int> classes = sample->getClasses();
    std::vector<size_t> classDistribution = sample->getClassesDistribution();
    std::vector<DataPointer<T> > points_by_class(classes.size());
    std::vector<size_t> new_class_distribution(classes.size());

    for(size_t i = 0; i < classes.size(); i++){
        points_by_class[i] = std::make_shared<Data<T> >();
    }

    for(auto it = sample->begin(); it != sample->end(); it++){
        auto point = (*it);

        if(classes.size() > 2){
            points_by_class[point->y-1]->insertPoint(point);
        }else{
            if(point->y == -1){
                points_by_class[1]->insertPoint(point);
            }else{ 
                points_by_class[0]->insertPoint(point);
            }
        }
    }

    for(size_t i = 0; i < classes.size(); i++){
        new_class_distribution[i] = ((double)classDistribution[i]/sample->getSize())*sizes;
    }

    test_sample = std::make_shared<Data< T > >();
    train_sample = std::make_shared<Data< T > >();

    std::vector<size_t> markers(classes.size(), 0);
    for(size_t i = 0; i < fold; i++){
        for(size_t j = 0; j < points_by_class.size(); j++){
            for(size_t k = markers[j], l = 0; markers[j] < points_by_class[j]->getSize() && l < new_class_distribution[j]; k++, l++){
                if(i != fold-1){
                    train_sample->insertPoint((*points_by_class[j])[k]);
                }else{ 
                    test_sample->insertPoint((*points_by_class[j])[k]);
                }
                markers[j]++;
            }
        }
    }

    train_sample->shuffle();
    test_sample->shuffle();
}

template < typename T >
double Validation< T > ::kFold (int fold, int seed){
    size_t i = 0, j = 0, k = 0, size = sample->getSize();
    size_t fp = 0, fn = 0, tp = 0, tn = 0;
    double sizes = sample->getSize()/fold;
    double error = 0.0, func = 0.0, margin = 0.0;
    std::vector<double> error_arr(fold);
    std::vector<DataPointer< T > > folds(fold);
    auto classes = sample->getClasses();
    auto classDistribution = sample->getClassesDistribution();
    std::vector<size_t> markers(classes.size(), 0);
    std::vector<DataPointer< T > > points_by_class(classes.size());
    std::vector<size_t> new_class_distribution(classes.size());
    bool isPrimal = classifier->getFormulationString() == "Primal";


    for(size_t i = 0; i < classes.size(); i++){
        points_by_class[i] = std::make_shared<Data< T > >();
    }
    for(size_t i = 0; i < fold; i++){
        folds[i] = std::make_shared<Data< T > >();
    }
    
    for(auto it = sample->begin(); it != sample->end(); it++){
        auto point = (*it);
        if(classes.size() > 2){
            points_by_class[point->y-1]->insertPoint(point);
        }else{
            if(point->y == -1){
                points_by_class[1]->insertPoint(point);
            }else{ 
                points_by_class[0]->insertPoint(point);
            }
        }
    }
    
    for(size_t i = 0; i < classes.size(); i++){
        new_class_distribution[i] = ((double)classDistribution[i]/sample->getSize())*sizes;
    }

    for(size_t i = 0; i < folds.size(); i++){
        for(size_t j = 0; j < points_by_class.size(); j++){
            for(size_t k = markers[j], l = 0; markers[j] < points_by_class[j]->getSize() && l < new_class_distribution[j]; k++, l++){
                folds[i]->insertPoint((*points_by_class[j])[k]);
                markers[j]++;
            }
        }
        folds[i]->shuffle();
    }
    points_by_class.clear();
    new_class_distribution.clear();

    //Start cross-validation
    for(size_t fp = 0, fn = 0, tp = 0, tn = 0, j = 0; j < fold; ++j){
        auto test_sample = folds[j];
        DataPointer< T > train_sample = std::make_shared<Data< T > >();
        train_sample->setClasses(sample->getClasses());
        for(size_t i = 0; i < fold; i++){
            if(i != j){
                for(auto it = folds[i]->begin(); it != folds[i]->end(); it++){
                    auto point = (*it);
                    train_sample->insertPoint(point);
                }
            }
        }
        if(verbose){
            std::cout << "\nCross-Validation " << j + 1 << ": \n";
            std::cout << "Train points: " << train_sample->getSize() << std::endl;
            std::cout << "Test points: " << test_sample->getSize() << std::endl;
            std::cout << std::endl;
        }

        // Training phase
        classifier->setSamples(train_sample);
        if(!classifier->train()){
            if(verbose){
                std::cerr << "Error at " << fold << "-fold: The convergency wasn't reached at the set " << j+1 << "!\n";
            }
        }

        Solution s = classifier->getSolution();
        bool isPrimal = classifier->getFormulationString() == "Primal";

        if(isPrimal){
            size_t i = 0;
            double func = 0;
            for(auto it = test_sample->begin(); it != test_sample->end(); it++, i++){
                auto point = (*it);
                double _y = classifier->evaluate(*point);

                if(point->y != _y){
                    if(verbose > 1)
                        std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->y << std::endl;
                    error_arr[j]++;
                    if(point->y == -1) fp++; else fn++;
                }else{
                    if(verbose > 1)
                        std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->y << std::endl;
                    if(point->y == -1) tn++; else tp++;
                }
            }
        }else{
            DualClassifier< T > *dual = dynamic_cast<DualClassifier< T > *>(classifier);
            Kernel K(dual->getKernelType(), dual->getKernelParam());
            dMatrix *matrix;
            std::shared_ptr<Data< T > > traintest_sample(std::make_shared<Data< T > >());

            *traintest_sample = *test_sample;
            traintest_sample->join(train_sample);
            K.compute(traintest_sample);
            matrix = K.getKernelMatrixPointer();
            size_t i = 0;
            for(auto it = test_sample->begin(); it != test_sample->end(); it++, i++){
                auto point = (*it);
                double _y = classifier->evaluate(*point);

                if(point->y != _y){
                    if(verbose > 1)
                        std::cerr << "[" << i+1 << "x] function: " << _y << ", y: " << point->y << std::endl;
                    error_arr[j]++;
                    if(point->y == -1) fp++; else fn++;
                }else{
                    if(verbose > 1)
                        std::cerr << "[" << i+1 << "] function: " << _y << ", y: " << point->y << std::endl;
                    if(point->y == -1) tn++; else tp++;
                }
            }
            
            std::cout << std::endl;
        }

        if(verbose) std::cout << "Error " << j + 1 << ": " << error_arr[j] << " -- " << ((double)error_arr[j]/(double)folds[j]->getSize())*100.0f << "%\n";
        error += ((double)error_arr[j]/(double)folds[j]->getSize())*100.0f;
        this->solution.accuracy += (double)(tp + tn)/(double)(tp + tn + fp + fn);
        this->solution.precision += (double)tp/(double)(tp + fp);
        this->solution.recall += (double)tp/(double)(tp + fn);
        this->solution.tnrate += (double)tn/(double)(tn + fp);
        this->solution.falseNegative += fn;
        this->solution.falsePositive += fp;
        this->solution.trueNegative += tn;
        this->solution.truePositive += tp;

        train_sample = make_shared<Data< T > >();
        test_sample = make_unique<Data< T > >();
    }

    return (((double)error)/(double)fold);
}

template < typename T >
ValidationSolution Validation< T > ::validation(int fold, int qtde){
    int i = 0, k = 0, erro = 0, svcount = 0, test_size = test_sample->getSize(),
            train_size = train_sample->getSize(), train_dim = train_sample->getDim();
    size_t fp = 0, fn = 0, tp = 0, tn = 0;
    double error = 0, errocross = 0, func = 0.0, margin = 0, bias;
    vector<double> w;
    bool isPrimal = (classifier->getFormulationString() == "Primal");

    sample = train_sample;

    /*cross-validation*/
    if(qtde > 0)
    {
        for(errocross = 0, i = 0; i < qtde; i++)
        {
            if(verbose) cout << "\nExecucao " << i + 1 << " / " << qtde << ":\n";
            errocross += kFold(fold, i);
        }
        cout << "\n\nErro " << fold << "-Fold Cross Validation: " << errocross/qtde << "%\n";
        this->solution.accuracy /= qtde*fold;
        this->solution.precision /= qtde*fold;
        this->solution.recall /= qtde*fold;
        this->solution.tnrate /= qtde*fold;
        this->solution.falseNegative /= qtde*fold;
        this->solution.falsePositive /= qtde*fold;
        this->solution.trueNegative /= qtde*fold;
        this->solution.truePositive /= qtde*fold;
    }

    /*start final validation*/
    if(verbose)
    {
        cout << "\nFinal Validation:\n";
        cout << "Pts de Treino: " << train_sample->getSize() << "\n";
        cout << "Pts de Teste:  " << test_sample->getSize() << "\n";
    }

    //training
    classifier->setSamples(train_sample);
    classifier->setVerbose(0);

    if(!classifier->train()){
        if(verbose)
            cerr << "Validation error: The convergency wasn't reached in the training set!\n";
    }

    Solution s = classifier->getSolution();

    w = s.w;
    bias = s.bias;

    if(isPrimal){
        for(i = 0; i < test_size; ++i)
        {
            shared_ptr<Point< T > > p = test_sample->getPoint(i);
            for(func = bias, k = 0; k < train_dim; ++k)
                func += w[k] * p->x[k];

            if(p->y * func <= 0)
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
                erro++;
                if(p->y == -1) fp++; else fn++;
            }
            else
            {
                if(verbose > 1) cout << "["<< i+1 <<"] function: " << func << ", y: " << p->y  << "\n";
                if(p->y == -1) tn++; else tp++;
            }
            if(verbose) cout.flush();
        }
    }else{
        /*testing imadual and smo*/
        DualClassifier< T >  *dual = dynamic_cast<DualClassifier< T > *>(classifier);
        Kernel K(dual->getKernelType(), dual->getKernelParam());
        dMatrix matrix;
        shared_ptr<Data< T > > traintest_sample(make_shared<Data< T > >());

        *traintest_sample = *test_sample;
        traintest_sample->join(train_sample);

        K.compute(traintest_sample);
        matrix = K.getKernelMatrix();

        for(i = 0; i < test_size; ++i)
        {
            shared_ptr<Point< T > > p = test_sample->getPoint(i);
            for(func = bias, k = 0; k < train_size; ++k)
                func += train_sample->getPoint(k)->alpha * train_sample->getPoint(k)->y * matrix[k+test_size][i];

            if(p->y * func <= 0)
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
                erro++;
                if(p->y == -1) fp++; else fn++;
            }
            else
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
                if(p->y == -1) tn++; else tp++;
            }
            if(verbose) cout.flush();
        }
    }

    cout << "Validation Error: " << erro << " -- " << (double)erro/(double)test_sample->getSize()*100.0f << "%\n";
    error += ((double)erro/(double)test_sample->getSize())*100.0f;

    return this->solution;
}

template < typename T >
std::shared_ptr<Data< T > > Validation< T > ::getTestSample(){
    return test_sample;
}

template < typename T >
std::shared_ptr<Data< T > > Validation< T > ::getTrainSample(){
    return train_sample;
}

template < typename T >
Validation< T > ::Validation() : sample(std::make_shared<Data< T > >()), train_sample(std::make_shared<Data< T > >()), test_sample(std::make_shared<Data< T > >()), classifier(nullptr) {}

template < typename T >
void Validation< T > ::setSamples(shared_ptr<Data<T>> sample) {
    this->sample = sample;
}

template < typename T >
void Validation< T > ::setSeed(unsigned int seed) {
    this->seed = seed;
}

template<typename T>
std::vector<std::vector<size_t> > Validation<T>::generateConfusionMatrix(Learner<T> &learner, Data<T> &samples) {
    auto classes = samples.getClasses();
    size_t size = samples.getSize(), i, j, idp, idy, n_classes = classes.size();
    std::vector<std::vector<size_t> > confusion_m(n_classes, std::vector<size_t>(n_classes, 0));
    double acc = 0.0;

    for(i = 0; i < size; i++){
        size_t pred = learner.evaluate(*(samples[i]));
        for(j = 0, idp = 0, idy = 0; j < n_classes; j++){
            if(classes[j] == pred){
                idp = j;
            }
            if(classes[j] == samples[i]->y){
                idy = j;
            }
            if(idp && idy) break;
        }
        confusion_m[idp][idy]++;
    }

    for(i = 0; i < confusion_m.size(); i++){
        acc += *std::max_element(confusion_m[i].begin(), confusion_m[i].end());
    }
    std::cout << "Purity: " << acc / size << std::endl;
    return confusion_m;
}


template class Validation<int>;
template class Validation<double>;
template class Validation<float>;
template class Validation<int8_t>;
template class Validation<char>;
template class Validation<long long int>;
template class Validation<short int>;
template class Validation<long double>;
template class Validation<unsigned char>;
template class Validation<unsigned int>;
template class Validation<unsigned short int>;