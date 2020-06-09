
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_FeatureSelection_include_FeatureSelection.hpp:

Program Listing for File FeatureSelection.hpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_FeatureSelection_include_FeatureSelection.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/FeatureSelection/include/FeatureSelection.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by Mateus Coutinho Marim on 6/26/2018.
   //
   
   #ifndef UFJF_MLTK_FEATURESELECTION_H
   #define UFJF_MLTK_FEATURESELECTION_H
   
   #include "Data.hpp"
   #include "Classifier.hpp"
   #include "Validation.hpp"
   
   #include <memory>
   
   template < typename T >
   class FeatureSelection {
   protected:
       std::shared_ptr<Data< T > > samples;
       Classifier< T > *classifier;
       typename Validation< T >::CrossValidation *cv;
       Solution solution;
       Timer timer;
   
       const int MAX_DEPTH = 99999;
       const int MAX_BREATH = 99999;
       const double NUM_ERROR_EPS = 0.05;
       int depth = 0;
       int final_dim = 0;
       int jump = 0;
       int skip = 0;
       bool leave_one_out = false;
       int verbose = 0;
   
   public:
   
       virtual std::shared_ptr<Data< T > > selectFeatures() = 0;
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setSamples(const std::shared_ptr<Data< T > > &samples) { this->samples = samples; }
       void setClassifier(Classifier< T > *classifier) { this->classifier = classifier; this->classifier->setVerbose(0); }
       void setFinalDimension(int final_dim){ this->final_dim = final_dim; }
       void setDepth(int depth) { this->depth = depth; }
       void setJump(int jump) { this->jump = jump; }
       void setSkip(int skip) { this->skip = skip; }
       void setVerbose(int verbose){ this->verbose = verbose; }
       void setCrossValidation(typename Validation< T >::CrossValidation *cv){ this->cv = cv; }
   };
   
   #endif //UFJF_MLTK_FEATURESELECTION_H
