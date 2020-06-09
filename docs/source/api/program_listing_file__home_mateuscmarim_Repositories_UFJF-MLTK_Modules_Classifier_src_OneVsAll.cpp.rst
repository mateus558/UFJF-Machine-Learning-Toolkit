
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_src_OneVsAll.cpp:

Program Listing for File OneVsAll.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_src_OneVsAll.cpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/src/OneVsAll.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 30/03/2020.
   //
   
   #include "OneVsAll.hpp"
   
   
   template< typename T, template <typename > class ClassifierT>
   OneVsAll<T, ClassifierT>::OneVsAll(Data< T > _samples, int _verbose) {
       this->samples = _samples;
       this->verbose = _verbose;
   }
   
   template< typename T, template <typename > class ClassifierT>
   bool OneVsAll<T, ClassifierT>::train() {
       auto classes = this->samples->getClasses();
       std::vector<int> new_classes = {-1, 1};
       size_t i, j, n_classes = classes.size(), size = this->samples->getSize();
   
       for(i = 0; i < n_classes; i++){
           auto temp_samples = this->samples->copy();
           temp_samples.setClasses(new_classes);
   
           for(j = 0; j < size; j++){
               temp_samples[j]->y = (temp_samples[j]->y == classes[i])?1:-1;
           }
           classifier.setMaxTime(this->max_time);
           classifier.setLearningRate(this->rate);
           classifier.setMaxIterations(this->MAX_IT);
           classifier.setEPS(this->EPS);
           classifier.setVerbose(this->verbose);
           classifier.setSamples(std::make_shared<Data< T > >(temp_samples));
   
           classifier.train();
   
           weights.push_back(classifier.getSolutionRef()->w);
           biases.push_back(classifier.getSolutionRef()->bias);
           classifier.getSolutionRef()->w.clear();
           classifier.getSolutionRef()->bias = 0;
       }
   
       return true;
   }
   
   template< typename T, template <typename > class ClassifierT>
   double OneVsAll<T, ClassifierT>::evaluate(Point<T> p) {
       return 0;
   }
   
   template< typename T, template <typename > class ClassifierT>
   std::string OneVsAll<T, ClassifierT>::getFormulationString() {
       return classifier.getFormulationString();
   }
