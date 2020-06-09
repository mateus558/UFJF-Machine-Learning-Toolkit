
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Validation_src_Validation.cpp:

Program Listing for File Validation.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Validation_src_Validation.cpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Validation/src/Validation.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
       size_t i, j, npos, nneg, size = sample->getSize();
       shared_ptr<Point< T > > p, aux;
       Data< T > sample_pos, sample_neg;
   
       sample_pos.copyZero(*sample);
       sample_neg.copyZero(*sample);
   
       for(i = 0; i < size; i++){
           p = sample->getPoint(i);
           if(p->y == 1){
               sample_pos.insertPoint(*sample, i);
           }else{
               sample_neg.insertPoint(*sample, i);
           }
       }
   
       npos = sample_pos.getSize();
       nneg = sample_neg.getSize();
   
       for(i = 0; i < npos; i++){
           j = Random::intInRange(0, npos-1);
           aux = sample_pos.getPoint(i);
           sample_pos.setPoint(i, sample_pos.getPoint(j));
           sample_pos.setPoint(j, aux);
       }
       for(i = 0; i < nneg; i++){
           j = Random::intInRange(0, nneg-1);
           aux = sample_neg.getPoint(i);
           sample_neg.setPoint(i, sample_neg.getPoint(j));
           sample_neg.setPoint(j, aux);
       }
   
       train_sample->copyZero(sample_pos);
       test_sample->copyZero(sample_pos);
   
       for(j = 0; j < npos*(fold-1)/fold; j++)
           train_sample->insertPoint(sample_pos, j);
       for(; j < npos; j++)
           test_sample->insertPoint(sample_pos, j);
   
       for(j = 0; j < nneg/fold; j++)
           test_sample->insertPoint(sample_neg, j);
       for(; j < nneg; j++)
           train_sample->insertPoint(sample_neg, j);
   }
   
   template < typename T >
   double Validation< T > ::kFold (int fold, int seed){
       size_t i = 0, j = 0, k = 0, size = sample->getSize();
       size_t qtdpos = 0, qtdneg = 0, cost_pos = 0, cost_neg = 0, svcount = 0;
       size_t fp = 0, fn = 0, tp = 0, tn = 0;
       double error = 0.0, func = 0.0, margin = 0.0;
       vector<double> w;
       vector<int> error_arr(fold);
       unique_ptr<Data< T > > sample_pos(make_unique<Data< T > > ()), sample_neg(make_unique<Data< T > > ()), test_sample(make_unique<Data< T > > ());
       shared_ptr<Data< T > >  train_sample(make_shared<Data< T > > ());
       vector<std::unique_ptr<Data< T > > > vet_sample_pos(fold), vet_sample_neg(fold), vet_sample_final(fold);
       bool isPrimal = classifier->getFormulationString() == "Primal";
   
       Random::init(seed);
   
       sample_neg->copyZero(*sample);
       sample_pos->copyZero(*sample);
   
       for(i = 0; i < size; i++){
           shared_ptr<Point< T > > p = sample->getPoint(i);
           if(p->y == 1)
               sample_pos->insertPoint(p);
           else
               sample_neg->insertPoint(p);
       }
   
       qtdpos = sample_pos->getSize();
       qtdneg = sample_neg->getSize();
   
       if(verbose){
           cout << "\nTotal of points: " << sample->getSize() << endl;
           cout << "Qtd of positive: " << qtdpos << endl;
           cout << "Qtd of negative: " << qtdneg << endl;
       }
   
       //randomize
       for(i = 0; i < qtdpos; ++i){
           shared_ptr<Point< T > > aux;
   
           j = Random::intInRange(0, sample_pos->getSize()-1);
           aux = sample_pos->getPoint(i);
           sample_pos->setPoint(i, sample_pos->getPoint(j));
           sample_pos->setPoint(j, aux);
       }
   
       for(i = 0; i < qtdneg; ++i){
           shared_ptr<Point< T > > aux;
   
           j = Random::intInRange(0, sample_neg->getSize()-1);
           aux = sample_neg->getPoint(i);
           sample_neg->setPoint(i, sample_neg->getPoint(j));
           sample_neg->setPoint(j, aux);
       }
   
       for(i = 0; i < fold; ++i){
           vet_sample_pos[i] = make_unique<Data< T > >();
           vet_sample_neg[i] = make_unique<Data< T > >();
           vet_sample_final[i] = make_unique<Data< T > >();
   
           vet_sample_pos[i]->copyZero(*sample);
           vet_sample_neg[i]->copyZero(*sample);
           vet_sample_final[i]->copyZero(*sample);
       }
   
       for(i = 0, j = 0; i < fold - 1; ++i){
           for(; j < (qtdpos-cost_pos)/(fold-i)+cost_pos; ++j)
               vet_sample_pos[i]->insertPoint(*sample_pos, j);
           cost_pos = j;
       }
   
       for(; j < qtdpos; ++j){
           vet_sample_pos[i]->insertPoint(*sample_pos, j);
       }
   
       for(i = 0, j = 0; i < fold-1; ++i){
           for(; j < (qtdneg-cost_neg)/(fold-i)+cost_neg; ++j)
               vet_sample_neg[fold-1-i]->insertPoint(*sample_neg, j);
           cost_neg = j;
       }
   
       for(; j < qtdneg; ++j)
           vet_sample_neg[fold-1-i]->insertPoint(*sample_neg, j);
   
       sample_pos.reset(nullptr);
       sample_neg.reset(nullptr);
   
       for(i = 0; i < fold; ++i){
           for(j = 0; j < vet_sample_pos[i]->getSize(); ++j)
               vet_sample_final[i]->insertPoint(*vet_sample_pos[i], j);
           for(; j < vet_sample_pos[i]->getSize() + vet_sample_neg[i]->getSize(); ++j)
               vet_sample_final[i]->insertPoint(*vet_sample_neg[i], j - vet_sample_pos[i]->getSize());
       }
   
       for(i = 0; i < fold; ++i){
           vet_sample_pos[i].reset(nullptr);
           vet_sample_neg[i].reset(nullptr);
       }
   
       vet_sample_pos.clear();
       vet_sample_neg.clear();
   
       //Start cross-validation
       for(fp = 0, fn = 0, tp = 0, tn = 0, j = 0; j < fold; ++j){
           test_sample->copy(*vet_sample_final[j]);
           train_sample->copyZero(*sample);
   
           for(i = 0; i < fold; ++i){
               if(i != j)
                   for(k = 0; k < vet_sample_final[i]->getSize(); ++k)
                       train_sample->insertPoint(*vet_sample_final[i], k);
           }
   
           if(verbose){
               cout << "\nCross-Validation " << j + 1 << ": \n";
               cout << "Train points: " << train_sample->getSize() << endl;
               cout << "Test points: " << test_sample->getSize() << endl;
               cout << endl;
           }
   
           //training
   
           classifier->setSamples(train_sample);
           if(!classifier->train()){
               if(verbose)
                   cerr << "Error at " << fold << "-fold: The convergency wasn't reached at the set " << j+1 << "!\n";
           }
   
           Solution s = classifier->getSolution();
   
           if(isPrimal){
               for(i = 0; i < test_sample->getSize(); ++i){
                   shared_ptr<Point< T > > p = test_sample->getPoint(i);
                   for(func = s.bias, k = 0; k < train_sample->getDim(); ++k)
                       func += s.w[k] * p->x[k];
   
                   if(p->y * func <= 0){
                       if(verbose > 1)
                           cerr << "[" << i+1 << "x] function: " << func << ", y: " << p->y << endl;
                       error_arr[j]++;
                       if(p->y == -1) fp++; else fn++;
                   }else{
                       if(verbose > 1)
                           cerr << "[" << i+1 << "] function: " << func << ", y: " << p->y << endl;
                       if(p->y == -1) tn++; else tp++;
                   }
               }
               cout << endl;
           }else{
               DualClassifier< T > *dual = dynamic_cast<DualClassifier< T > *>(classifier);
               Kernel K(dual->getKernelType(), dual->getKernelParam());
               dMatrix *matrix;
               shared_ptr<Data< T > > traintest_sample(make_shared<Data< T > >());
   
               *traintest_sample = *test_sample;
               traintest_sample->join(train_sample);
               K.compute(traintest_sample);
               matrix = K.getKernelMatrixPointer();
   
               for(i = 0; i < test_sample->getSize(); ++i){
                   for(func = s.bias, k = 0; k < train_sample->getSize(); ++k)
                       func += train_sample->getPoint(k)->alpha * train_sample->getPoint(k)->y * (*matrix)[k+test_sample->getSize()][i];
   
                   if(test_sample->getPoint(i)->y * func <= 0){
                       if(verbose > 1)
                           cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->getPoint(i)->y << endl;
                       error_arr[j]++;
                       if(test_sample->getPoint(i)->y == -1) fp++; else fn++;
                   }else{
                       if(verbose > 1)
                           cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->getPoint(i)->y << endl;
                       if(test_sample->getPoint(i)->y == -1) tn++; else tp++;
                   }
               }
               cout << endl;
           }
           if(verbose) cout << "Error " << j + 1 << ": " << error_arr[j] << " -- " << ((double)error_arr[j]/(double)vet_sample_final[j]->getSize())*100.0f << "%";
           error += ((double)error_arr[j]/(double)vet_sample_final[j]->getSize())*100.0f;
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
