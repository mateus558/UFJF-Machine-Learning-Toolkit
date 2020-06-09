
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_OneVsAll.hpp:

Program Listing for File OneVsAll.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_OneVsAll.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/OneVsAll.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 30/03/2020.
   //
   
   #ifndef UFJF_MLTK_ONEVSALL_HPP
   #define UFJF_MLTK_ONEVSALL_HPP
   
   #include "Classifier.hpp"
   
   template< typename T, template <typename > class ClassifierT>
   class OneVsAll: public Classifier< T > {
   private:
       ClassifierT< T > classifier;
       std::vector<std::vector<double> > weights;
       std::vector<double> biases;
   
   public:
       OneVsAll(Data< T > _samples, int _verbose);
   
       bool train() override;
   
       double evaluate(Point<T> p) override;
   
       std::string getFormulationString() override;
   };
   
   
   #endif //UFJF_MLTK_ONEVSALL_HPP
