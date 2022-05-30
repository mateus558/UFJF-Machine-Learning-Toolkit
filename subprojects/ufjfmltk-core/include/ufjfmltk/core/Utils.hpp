/*! Utils functions
   \file Utils.hpp
   \author Mateus Coutinho Marim
*/


#pragma once
#define PRECISION 1E-9
#define MAX_NUMBER_STRING_SIZE 32
#define INF 1E8

#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <iterator>
#include <ctime>

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

        // trim from start (in place)
        static inline void ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
        }

// trim from end (in place)
        static inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

// trim from both ends (in place)
        static inline void trim(std::string &s) {
            ltrim(s);
            rtrim(s);
        }

// trim from start (copying)
        static inline std::string ltrim_copy(std::string s) {
            ltrim(s);
            return s;
        }

// trim from end (copying)
        static inline std::string rtrim_copy(std::string s) {
            rtrim(s);
            return s;
        }

// trim from both ends (copying)
        static inline std::string trim_copy(std::string s) {
            trim(s);
            return s;
        }
    }
}
