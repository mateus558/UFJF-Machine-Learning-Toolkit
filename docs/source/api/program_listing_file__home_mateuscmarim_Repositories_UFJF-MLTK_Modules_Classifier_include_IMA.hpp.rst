
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_IMA.hpp:

Program Listing for File IMA.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_IMA.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/IMA.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   //
   // Created by mateus558 on 01/08/17.
   //
   
   #ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
   #define CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
   
   #include <vector>
   #include "PrimalClassifier.hpp"
   #include "DualClassifier.hpp"
   #include "MLToolkit.hpp"
   
   template < typename T >
   class IMAp : public PrimalClassifier< T > {
   private:
       double margin;
       std::vector<int> svs;
   
   public:
       explicit IMAp(std::shared_ptr<Data< T > > samples = nullptr, double margin = 0.0, Solution *initial_solution = nullptr);
   
       bool train() override;
       double evaluate(Point< T >  p) override;
       std::vector<int> getSupportVectors(){ return svs; }
   };
   
   template < typename T >
   class IMApFixedMargin : public PrimalClassifier< T >  {
   private:
       int n, maiorn = 0, flagNao1aDim;
       double margin = 0.0, maiorw = 0.0;
       unsigned long tMax;
   
       std::vector<int> svs;
   
   public:
       explicit IMApFixedMargin(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 0, Solution *initial_solution = nullptr);
   
       bool train() override;
       double evaluate(Point< T >  p) override;
       inline int* getFlagNot1aDim() {return &flagNao1aDim; }
       inline unsigned long* gettMax(){ return &tMax; }
   };
   
   template < typename T >
   class IMADual : public DualClassifier< T >  {
   private:
       double margin = 0;
       std::vector<int> svs;
   public:
       explicit IMADual(std::shared_ptr<Data< T > > samples = nullptr, Kernel *k = nullptr, double rate = 1, Solution *initial_solution = nullptr);
       bool train() override;
       std::vector<int> getSupportVectors(){ return svs; }
   };
   
   #endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
