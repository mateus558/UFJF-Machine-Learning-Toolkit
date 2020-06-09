
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_Perceptron.hpp:

Program Listing for File Perceptron.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_Perceptron.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/Perceptron.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef PERCEPTRONPRIMAL__HPP
   #define PERCEPTRONPRIMAL__HPP
   
   #include "PrimalClassifier.hpp"
   #include "DualClassifier.hpp"
   #include "MLToolkit.hpp"
   
   template < typename T >
   class PerceptronPrimal : public PrimalClassifier< T >  {
   public:
       explicit PerceptronPrimal(std::shared_ptr<Data< T > > samples = nullptr, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
       bool train() override;
   };
   
   template < typename T >
   class PerceptronFixedMarginPrimal : public PrimalClassifier< T >  {
   public:
       explicit PerceptronFixedMarginPrimal(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
       bool train() override;
       double evaluate(Point< T > p) override;
   };
   
   template < typename T >
   class PerceptronDual : public DualClassifier< T >  {
   public:
       explicit PerceptronDual(std::shared_ptr<Data< T > > samples = nullptr, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
       bool train() override;
   };
   
   template < typename T >
   class PerceptronFixedMarginDual : public DualClassifier< T >  {
   public:
       explicit PerceptronFixedMarginDual(std::shared_ptr<Data< T > > samples = nullptr, double gamma = 1.0, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
       bool train() override;
   };
   
   #endif
