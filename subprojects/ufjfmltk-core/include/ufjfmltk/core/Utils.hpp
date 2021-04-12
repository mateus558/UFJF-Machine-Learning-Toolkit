/*! Utils functions
   \file Utils.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLUtils __declspec(dllexport)
    #define DLLUtilsDecl __cdecl
  #else
    #define DLLUtils
    #define DLLUtilsDecl
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLUtils __attribute__ ((visibility ("default")))
      #define DLLUtilsDecl
  #else
      #define DLLUtils
      #define DLLUtilsDecl
  #endif
#endif

#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#define INF 1E8

namespace mltk{
    enum NormType {NORM_LINF = 0, NORM_L1 = 1, NORM_L2 = 2};
    typedef std::vector<std::vector<double> > dMatrix;

    namespace utils {
        DLLUtils void DLLUtilsDecl printConfusionMatrix(std::vector<int> &classes, std::vector<std::string> classes_names, std::vector<std::vector<size_t> > &confusion_m, bool show_names=false);
    /**
     * \brief Verify if the string is a number.
     * \param str   String to be tested.
     * \return  bool
     */
        DLLUtils bool DLLUtilsDecl is_number(std::string str);

    /**
     * \brief Converts the string to an integer.
     * \param str   String to be converted.
     * \return The integer resulted from the conversion.
     */
       DLLUtils int DLLUtilsDecl stoin(std::string str);

    /**
     * \brief Converts the string to a double.
     * \param str   The string to be converted.
     * \return  The double resulted from the conversion.
     */
       DLLUtils double DLLUtilsDecl atod(const char* str);

    /**
     * \brief Returns the max absolute element.
     * \param x The vector used to obtain the max element.
     * \return  The max absolute element found.
     */
        template<typename T>
       DLLUtils double DLLUtilsDecl maxAbsElement(std::vector<T> x) {
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

    /**
     * \brief itos Integer to string conversion.
     * \param n Integer to be converted.
     * \return string
     */
       DLLUtils std::string DLLUtilsDecl itos(int n);

    /**
     * \brief dtoa Double to string conversion.
     * \param n Double to be converted.
     * \return string
     */
       DLLUtils std::string DLLUtilsDecl dtoa(double n);

        template<typename Out>
       DLLUtils void DLLUtilsDecl split(const std::string &s, char delim, Out result);

       DLLUtils std::vector<std::string> DLLUtilsDecl split(const std::string &s, char delim);

    }
}
#endif