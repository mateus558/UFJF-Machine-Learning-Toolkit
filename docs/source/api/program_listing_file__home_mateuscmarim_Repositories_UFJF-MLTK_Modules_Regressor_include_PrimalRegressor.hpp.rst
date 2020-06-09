
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_PrimalRegressor.hpp:

Program Listing for File PrimalRegressor.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_PrimalRegressor.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Regressor/include/PrimalRegressor.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 03/11/18.
   //
   
   #ifndef UFJF_MLTK_PRIMALREGRESSOR_HPP
   #define UFJF_MLTK_PRIMALREGRESSOR_HPP
   
   #include "Regressor.hpp"
   
   template < typename T >
   class PrimalRegressor : public Regressor< T > {
       // Attributes
   protected :
       std::vector<double> w;
   
   public:
   
       double evaluate(Point< T > p) override {
           double func = 0.0;
           size_t i, dim = this->solution.w.size();
   
           if(p.x.size() != dim){
               std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
               return 0;
           }
   
           for(func = this->solution.bias, i = 0; i < dim; i++){
               func += this->solution.w[i] * p[i];
           }
   
           return func;
       }
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       std::string getFormulationString() override { return "Primal"; }
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
   };
   
   
   #endif //UFJF_MLTK_PRIMALREGRESSOR_HPP
