/*! Timer class
   \file Timer.hpp
   \author Mateus Coutinho Marim
*/

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <cstdio>
#include <ctime>

/**
 * \brief Wrapper for the implementation of a simple timer.
 */
class Timer {
public:
    /**
     * \brief Contructor already initiate the timer to the current time.
     */
    inline Timer() {start = std::clock();}
    /**
     * \brief Set the timer to the current time.
     */
    inline void Reset(){ start = std::clock(); }
    /**
     * \brief Returns the elapsed time.
     * \returns double
     */
    inline double Elapsed() const { return 100.0f*( std::clock() - start ) / (double) CLOCKS_PER_SEC; }
private:
    /// Initial time.
    std::clock_t start;
    /// Elapsed time.
    double duration;
};
#endif
