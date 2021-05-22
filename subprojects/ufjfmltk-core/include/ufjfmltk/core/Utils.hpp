/*! Utils functions
   \file Utils.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#define INF 1E8

namespace mltk{
    enum NormType {NORM_LINF = 0, NORM_L1 = 1, NORM_L2 = 2};
    typedef std::vector<std::vector<double> > dMatrix;

    namespace utils {
         void  printConfusionMatrix(const std::vector<int>& classes, const std::vector<std::string>& classes_names, const std::vector<std::vector<size_t> > &confusion_m, bool show_names=false);
    /**
     * \brief Verify if the string is a number.
     * \param str   String to be tested.
     * \return  bool
     */
         bool  is_number(std::string str);

    /**
     * \brief Converts the string to an integer.
     * \param str   String to be converted.
     * \return The integer resulted from the conversion.
     */
        int  stoin(std::string str);

    /**
     * \brief Converts the string to a double.
     * \param str   The string to be converted.
     * \return  The double resulted from the conversion.
     */
        double  atod(const char* str);

    /**
     * \brief itos Integer to string conversion.
     * \param n Integer to be converted.
     * \return string
     */
        std::string  itos(int n);

    /**
     * \brief dtoa Double to string conversion.
     * \param n Double to be converted.
     * \return string
     */
        std::string  dtoa(double n);

        std::vector<std::string> tokenize(const std::string &str, const char delim);

        std::string timestamp();
    }
}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../../../src/Utils.cpp"
#endif
#endif
