
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_PrimalClassifier.hpp:

Program Listing for File PrimalClassifier.hpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_PrimalClassifier.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/PrimalClassifier.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef PRIMALCLASSIFIER__HPP
   #define PRIMALCLASSIFIER__HPP
   
   #include "Classifier.hpp"
   
   template < typename T >
   class PrimalClassifier : public Classifier< T > {
       // Attributes
   protected :
       std::vector<double> w;
       double q = 2, p = 2;
   
   protected:
       double flexible = 0.0;
       double alpha_aprox = 0.0;
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
   
           return (func >= 0)?1:-1;
       }
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       std::string getFormulationString() override { return "Primal"; }
       double getP() const { return p; }
       double getQ() const { return q; }
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setqNorm(double q) {this->q = q;}
       void setpNorm(double p) {this->p = p;}
       void setFlexible(double flexible) {this->flexible = flexible;}
       void setAlphaAprox(double alpha_aprox) {this->alpha_aprox = alpha_aprox;}
   };
   
   #endif
