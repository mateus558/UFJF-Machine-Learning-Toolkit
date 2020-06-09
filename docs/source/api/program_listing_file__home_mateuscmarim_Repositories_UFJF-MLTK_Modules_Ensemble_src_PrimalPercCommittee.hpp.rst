
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_PrimalPercCommittee.hpp:

Program Listing for File PrimalPercCommittee.hpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_PrimalPercCommittee.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Ensemble/src/PrimalPercCommittee.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 29/07/19.
   //
   
   #ifndef UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP
   #define UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP
   
   #include "PrimalCommittee.hpp"
   #include "Perceptron.hpp"
   
   template <typename T>
   class PrimalPercCommittee: public PrimalCommittee<T> {
   private:
       PerceptronPrimal< T > *perc;
   public:
       explicit PrimalPercCommittee(std::shared_ptr<Data< T > > samples = nullptr, size_t committee_size= 1, PerceptronPrimal< T > *perc = nullptr);
       bool train();
   };
   
   
   #endif //UFJF_MLTK_PRIMALPERCCOMMITTEE_HPP
