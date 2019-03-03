/*! Utils functions
   \brief Implementation of methods for general use in the system.
   \file Utils.cpp
   \author Mateus Coutinho Marim
*/

#include <cmath>
#include <cstring>
#include <sstream>
#include "../includes/Utils.hpp"
#define PRECISION 1E-9
#define MAX_NUMBER_STRING_SIZE 32

using namespace std;

bool is_number(string str){
    int i = 0, size = str.size();

    if(int(str[i]) == 13) return false;

    for(i = 0; i < size; ++i){
        if(int(str[i]) < int('0') && int(str[i]) > int('9')){
            if(str[i] != '.' && str[i] != '-')
                return false;
        }
    }

    return true;
}

int stoin(string str){
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

double atof(char s[])
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

string itos(int n){
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

    return string(buffer);
}

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

string dtoa(double n){
    char s[MAX_NUMBER_STRING_SIZE];

    // handle special cases
    if (n != n) {
        strcpy(s, "nan");
    } else if (std::isinf(n)) {
        strcpy(s, "inf");
    } else if (n == 0.0) {
        strcpy(s, "0");
    } else {
        int digit, m, m1;
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
    return string(s);
}


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

double min(double a, double b){
    return (a < b)?a:b;
}

double max(double a, double b){
    return (a > b)?a:b;
}
