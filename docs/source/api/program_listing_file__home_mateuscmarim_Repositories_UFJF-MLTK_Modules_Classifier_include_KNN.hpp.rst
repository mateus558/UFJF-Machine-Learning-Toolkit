
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_KNN.hpp:

Program Listing for File KNN.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_KNN.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/KNN.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 20/03/2020.
   //
   
   #ifndef UFJF_MLTK_KNN_HPP
   #define UFJF_MLTK_KNN_HPP
   
   #include "PrimalClassifier.hpp"
   #include <assert.h>
   #include <functional>
   
   template <typename T>
   class KNN: public PrimalClassifier< T > {
   private:
       typedef std::function<double(std::shared_ptr<Point< T > >, std::shared_ptr<Point< T > >)> function;
   
       size_t k;
       function dist_function;
   
   public:
       KNN(std::shared_ptr<Data<T> > _samples, size_t _k, function _dist_function  = [] (const std::shared_ptr<Point< T > > p, const std::shared_ptr<Point< T > > q){
           const size_t _dimp = p->x.size();
           size_t i;
           double dist = 0;
   
           for(i = 0; i < _dimp; i++){
               dist += (p->x[i] - q->x[i]) * (p->x[i] - q->x[i]);
           }
           return sqrt(dist);
       });
   
       bool train() override;
   
       double evaluate(Point<T> p) override;
   };
   /*
   
   */
   #endif //UFJF_MLTK_KNN_HPP
