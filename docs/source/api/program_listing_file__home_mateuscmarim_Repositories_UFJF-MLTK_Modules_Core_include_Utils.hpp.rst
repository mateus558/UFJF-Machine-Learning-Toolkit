
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Utils.hpp:

Program Listing for File Utils.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Utils.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Utils.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef UTILS_H_INCLUDED
   #define UTILS_H_INCLUDED
   
   #include <vector>
   #include <string>
   #include <sstream>
   #include <iterator>
   #define INF 1E8
   
   enum NormType {NORM_LINF = 0, NORM_L1 = 1, NORM_L2 = 2};
   typedef std::vector<std::vector<double> > dMatrix;
   
   namespace Utils {
   
       void printConfusionMatrix(std::vector<int> &classes, std::vector<std::vector<size_t> > &confusion_m);
       bool is_number(std::string str);
   
       int stoin(std::string str);
   
       double atod(const char* str);
   
       template<typename T>
       double maxAbsElement(std::vector<T> x) {
           int i, dim = x.size();
           double max, absv;
   
           for (i = 0, max = -INF; i < dim; i++) {
               absv = fabs(x[i]);
               if (absv > max) {
                   max = absv;
               }
           }
   
           return max;
       }
   
       std::string itos(int n);
   
       std::string dtoa(double n);
   
       template<typename Out>
       void split(const std::string &s, char delim, Out result);
   
       std::vector<std::string> split(const std::string &s, char delim);
   
   }
   
   #endif
