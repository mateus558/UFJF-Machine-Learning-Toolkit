
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Solution.hpp:

Program Listing for File Solution.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Solution.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Solution.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef SOLUTION__HPP
   #define SOLUTION__HPP
   
   #include <vector>
   #include "Kernel.hpp"
   
   class Solution {
       // Attributes
   public :
       std::vector<double> w, func;
       Kernel K;
       std::vector<double> alpha;
       double bias = 0;
       std::vector<int> fnames;
       double margin = 0;
       double norm = 0;
       unsigned int svs = 0;
   
       Solution& operator=(const Solution& other){
           w = other.w;
           func = other.func;
           K = other.K;
           alpha = other.alpha;
           bias = other.bias;
           fnames = other.fnames;
           margin = other.margin;
           norm = other.norm;
           svs = other.svs;
   
           return *this;
       }
   };
   
   #endif
