
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_PrimalCommittee.hpp:

Program Listing for File PrimalCommittee.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_PrimalCommittee.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Ensemble/src/PrimalCommittee.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 29/07/19.
   //
   
   #ifndef UFJF_MLTK_PRIMALCOMMITTEE_HPP
   #define UFJF_MLTK_PRIMALCOMMITTEE_HPP
   
   #include "Ensemble.hpp"
   
   template <typename T>
   class PrimalCommittee: public Ensemble<T> {
   protected:
       std::vector<std::vector<double> > w;
       std::vector<double> bias;
       size_t permutation;
       double similarity;
   public:
   
       double evaluate(Point< T > p) {
           return 0.0;
       }
   };
   
   
   #endif //UFJF_MLTK_PRIMALCOMMITTEE_HPP
