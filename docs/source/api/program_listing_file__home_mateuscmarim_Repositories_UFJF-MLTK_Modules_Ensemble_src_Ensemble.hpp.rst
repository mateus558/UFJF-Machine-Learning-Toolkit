
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_Ensemble.hpp:

Program Listing for File Ensemble.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Ensemble_src_Ensemble.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Ensemble/src/Ensemble.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 29/07/19.
   //
   
   #ifndef UFJF_MLTK_ENSEMBLE_HPP
   #define UFJF_MLTK_ENSEMBLE_HPP
   
   #include "Learner.hpp"
   
   template <typename T>
   class Ensemble: public Learner< T > {
       // Attributes
   protected:
       size_t c_size;
       Learner< T > *learner;
       Solution solution;
   public:
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       Solution getSolution() const {return solution;}
       Solution *getSolutionRef() { return &solution; }
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setLearner(Learner<T> *learner) {
           Ensemble::learner = learner;
       }
   
       void setCommitteeSize(size_t c_size) {
           Ensemble::c_size = c_size;
       }
   
   };
   
   
   #endif //UFJF_MLTK_ENSEMBLE_HPP
