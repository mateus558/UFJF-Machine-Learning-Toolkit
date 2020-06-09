
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_Regressor.hpp:

Program Listing for File Regressor.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_Regressor.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Regressor/include/Regressor.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 18/10/18.
   //
   
   #ifndef UFJF_MLTK_REGRESSOR_H
   #define UFJF_MLTK_REGRESSOR_H
   
   #endif //UFJF_MLTK_REGRESSOR_H
   
   #include "Learner.hpp"
   
   template <typename T >
   class Regressor: public Learner<T>{
   protected:
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
   
       void setW(std::vector<double> w) {this->solution.w = w;}
       void setSolution(Solution solution) {this->solution = solution;}
   };
