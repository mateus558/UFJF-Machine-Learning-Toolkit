
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Validation_include_Validation.hpp:

Program Listing for File Validation.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Validation_include_Validation.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Validation/include/Validation.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef VALIDATION__HPP
   #define VALIDATION__HPP
   
   #include "Classifier.hpp"
   #include "Data.hpp"
   #include "Solution.hpp"
   
   struct ValidationSolution : public Solution {
       // Attributes
       double accuracy = 0.0;
       double precision = 0.0;
       double recall = 0.0;
       double tnrate = 0.0;
       double tprate = 0.0;
       size_t falsePositive = 0;
       size_t falseNegative = 0;
       size_t trueNegative = 0;
       size_t truePositive = 0;
   };
   
   template < typename T >
   class Validation {
       // Attributes
   private :
       Classifier< T >  *classifier;
       std::shared_ptr<Data< T > > sample;
       std::shared_ptr<Data< T > > train_sample, test_sample;
       double initial_error, limit_error, actual_error;
       double kernel_param;
       int kernel_type;
       int verbose = 0;
       unsigned int seed;
       ValidationSolution solution;
   
       // Operations
   public :
       struct CrossValidation{
           size_t qtde = 0;
           int fold = 0;
           int jump = 0;
           std::vector<unsigned int> seed;
   
           double initial_error = 0.0;
           double actual_error = 0.0;
           double limit_error = 0.0;
       };
   
       Validation ();
       explicit Validation (std::shared_ptr<Data< T > > sample = std::make_shared<Data< T > >(), Classifier< T >  *classifier = nullptr, unsigned int seed = 666);
   
       static std::vector<std::vector<size_t> > generateConfusionMatrix(Learner< T > &learner, Data< T > &samples);
       void partTrainTest(int fold);
       double kFold (int fold, int seed);
       ValidationSolution validation (int fold, int qtde);
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       std::shared_ptr<Data< T > > getTestSample ();
       std::shared_ptr<Data< T > > getTrainSample ();
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setSamples(std::shared_ptr<Data< T > > sample);
       void setVerbose(int verbose){this->verbose = verbose;}
       void setSeed(unsigned int seed);
   };
   
   #endif
