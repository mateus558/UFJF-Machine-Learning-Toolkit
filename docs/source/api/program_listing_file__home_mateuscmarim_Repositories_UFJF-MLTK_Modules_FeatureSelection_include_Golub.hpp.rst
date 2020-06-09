
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_FeatureSelection_include_Golub.hpp:

Program Listing for File Golub.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_FeatureSelection_include_Golub.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/FeatureSelection/include/Golub.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by Mateus Coutinho Mari on 7/27/2018.
   //
   
   #ifndef UFJF_MLTK_GOLUB_H
   #define UFJF_MLTK_GOLUB_H
   
   #include "FeatureSelection.hpp"
   
   template < typename T >
   class Golub: public FeatureSelection< T > {
   private:
       int number;
   
       struct golub_select_score
       {
           int fname;
           double score;
       };
       static int golub_select_compare_score_greater(const golub_select_score &a, const golub_select_score &b);
   public:
       Golub(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T > *classifier = nullptr, int number = 0);
       std::shared_ptr<Data< T > > selectFeatures() override;
   };
   
   
   #endif //UFJF_MLTK_GOLUB_H
