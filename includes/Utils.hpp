/*! Utils functions
   \file Utils.hpp
   \author Mateus Coutinho Marim
*/

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#define INF 1E8

enum NormType {NORM_LINF = 0, NORM_L1 = 1, NORM_L2 = 2};
typedef std::vector<std::vector<double> > dMatrix;

/**
 * \brief Verify if the string is a number.
 * \param str   String to be tested.
 * \return  bool
 */
bool is_number(std::string str);
/**
 * \brief Converts the string to an integer.
 * \param str   String to be converted.
 * \return The integer resulted from the conversion.
 */
int stoin(std::string str);
/**
 * \brief Converts the string to a double.
 * \param str   The string to be converted.
 * \return  The double resulted from the conversion.
 */
double stodn(std::string str);
/**
 * \brief Returns the max absolute element.
 * \param x The vector used to obtain the max element.
 * \return  The max absolute element found.
 */
template < typename T >
double maxAbsElement(std::vector< T > x){
    int i, dim = x.size();
    double max, absv;

    for(i = 0, max = -INF; i < dim; i++){
        absv = fabs(x[i]);
        if(absv > max){
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
std::string itos(int n);
/**
 * \brief dtoa Double to string conversion.
 * \param n Double to be converted.
 * \return string
 */
std::string dtoa(double n);

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

double min(double a, double b);

double max(double a, double b);


#endif
