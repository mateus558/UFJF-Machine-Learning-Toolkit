
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_Classifier.hpp:

Program Listing for File Classifier.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_Classifier.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/Classifier.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef CLASSIFIER__HPP
   #define CLASSIFIER__HPP
   
   #include "Learner.hpp"
   
   template < typename T >
   class Classifier: public Learner< T > {
       // Attributes
   protected :
       bool hasInitialSolution = false;
       std::vector<Point< T > > svs;
       Solution solution;
       double gamma = 0;
       
       // Operations
   public :
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       Solution getSolution() const {return solution;}
       Solution *getSolutionRef() { return &solution; }
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       inline void setGamma(double gamma) { this->gamma = gamma;}
       void setW(std::vector<double> w) {this->solution.w = w;}
       void setSolution(Solution solution) {this->solution = solution;}
   };
   
   #endif
