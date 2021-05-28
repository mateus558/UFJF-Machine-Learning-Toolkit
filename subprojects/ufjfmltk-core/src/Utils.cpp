/*! Utils functions
   \brief Implementation of methods for general use in the system.
   \file Utils.cpp
   \author Mateus Coutinho Marim
*/
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "ufjfmltk/core/Utils.hpp"

using namespace std;

namespace mltk::utils{
        bool is_number(std::string s){
            return !s.empty() && std::find_if(s.begin(),
                                              s.end(), [](unsigned char c) { return !(std::isdigit(c) || c == '.' || c == '-' || c == '+' || c == 'e'); }) == s.end();
        }

        int stoin(std::string str){
            int i = 1, m = 1, num = 0, sign = 0, size = str.size();

            if(str[0] == '0'){
                return 0;
            }else if(str[0] == '-'){
                sign = 1;
            }

            for(i = i + sign; i < size; ++i)
                m *= 10;

            for(i = sign; i < size; ++i){
                num += int(str[i] - '0')*m;
                m /= 10;
            }

            return (sign) ? num * -1 : num;
        }

        double atod(const char* s)
        {
            double val, power;
            int i, sign;
            for (i = 0; isspace(s[i]); i++) /* skip white space */
                ;
            sign = (s[i] == '-') ? -1 : 1;
            if (s[i] == '+' || s[i] == '-')
                i++;
            for (val = 0.0; isdigit(s[i]); i++)
                val = 10.0 * val + (s[i] - '0');
            if (s[i] == '.')
                i++;
            for (power = 1.0; isdigit(s[i]); i++) {
                val = 10.0 * val + (s[i] - '0');
                power *= 10;
            }
            return sign * val / power;
        }

        std::string itos(int n){
            char buffer[50];
            int i = 0;

            bool isNeg = n<0;

            unsigned int n1 = isNeg ? -n : n;

            while(n1!=0){
                buffer[i++] = n1%10+'0';
                n1=n1/10;
            }

            if(isNeg)
                buffer[i++] = '-';

            buffer[i] = '\0';

            for(int t = 0; t < i/2; t++)
            {
                buffer[t] ^= buffer[i-t-1];
                buffer[i-t-1] ^= buffer[t];
                buffer[t] ^= buffer[i-t-1];
            }

            if(n == 0)
            {
                buffer[0] = '0';
                buffer[1] = '\0';
            }

            return std::string(buffer);
        }

        std::string dtoa(double n){
            char s[MAX_NUMBER_STRING_SIZE];

            // handle special cases
            if (n != n) {
                std::strcpy(s, "nan");
            } else if (std::isinf(n)) {
                std::strcpy(s, "inf");
            } else if (n == 0.0) {
                std::strcpy(s, "0");
            } else {
                int digit, m, m1 = 0;
                char *c = s;
                int neg = (n < 0);
                if (neg)
                    n = -n;
                // calculate magnitude
                m = log10(n);
                int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
                if (neg)
                    *(c++) = '-';
                // set up for scientific notation
                if (useExp) {
                    if (m < 0)
                        m -= 1.0;
                    n = n / pow(10.0, m);
                    m1 = m;
                    m = 0;
                }
                if (m < 1.0) {
                    m = 0;
                }
                // convert the number
                while (n > PRECISION || m >= 0) {
                    double weight = pow(10.0, m);
                    if (weight > 0 && !std::isinf(weight)) {
                        digit = floor(n / weight);
                        n -= (digit * weight);
                        *(c++) = '0' + digit;
                    }
                    if (m == 0 && n > 0)
                        *(c++) = '.';
                    m--;
                }
                if (useExp) {
                    // convert the exponent
                    int i, j;
                    *(c++) = 'e';
                    if (m1 > 0) {
                        *(c++) = '+';
                    } else {
                        *(c++) = '-';
                        m1 = -m1;
                    }
                    m = 0;
                    while (m1 > 0) {
                        *(c++) = '0' + m1 % 10;
                        m1 /= 10;
                        m++;
                    }
                    c -= m;
                    for (i = 0, j = m-1; i<j; i++, j--) {
                        // swap without temporary
                        c[i] ^= c[j];
                        c[j] ^= c[i];
                        c[i] ^= c[j];
                    }
                    c += m;
                }
                *(c) = '\0';
            }
            return std::string(s);
        }

        void printConfusionMatrix(const std::vector<int> &classes, const std::vector<std::string>& classes_names, const std::vector<std::vector<size_t> > &confusion_m, bool show_names) {
            size_t i, j, n_classes = classes.size();
            std::cout << "    ";
            for(i = 0; i < n_classes; i++){
                if(!show_names) {
                    std::cout << classes[i] << "\t";
                }else{
                    std::cout << classes_names[i] << "\t";
                }
            }
            std::cout << std::endl;
            std::cout << "    ";
            for(i = 0; i < n_classes; i++){
                std::cout << "----";
            }
            std::cout << std::endl;
            for(i = 0; i < n_classes; i++){
                if(!show_names) {
                    std::cout << classes[i] << " | ";
                }else {
                    std::cout << classes_names[i] << " | ";
                }
                for(j = 0; j < n_classes; j++){
                    std::cout << confusion_m[i][j] << "\t";
                }
                std::cout << std::endl;
            }
        }

        std::vector<std::string> tokenize(const std::string &str, const char delim)
        {
            std::vector<std::string> out;
            size_t start;
            size_t end = 0;

            while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
            {
                end = str.find(delim, start);
                out.push_back(str.substr(start, end - start));
            }

            return out;
        }

        std::string timestamp(){
            time_t rawtime;
            std::time(&rawtime);
            struct tm *tinfo = std::localtime(&rawtime);
            char buffer[21];
            strftime(buffer, 21, "%F-%X", tinfo);
            return std::string(buffer);
        }

    }