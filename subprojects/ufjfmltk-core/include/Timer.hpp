/*! Timer class
   \file Timer.hpp
   \author Mateus Coutinho Marim
*/

#pragma once
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLTimer __declspec(dllexport)
  #else
    #define DLLTimer __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLTimer __attribute__ ((visibility ("default")))
  #else
      #define DLLTimer
  #endif
#endif

#include <cstdio>
#include <ctime>
#include <chrono>

namespace mltk{
    /**
     * \brief Wrapper for the implementation of a simple timer.
     */
    class DLLTimer Timer {
    public:
        /**
         * \brief Contructor already initiate the timer to the current time.
         */
        inline Timer() {start = std::chrono::high_resolution_clock::now();}
        /**
         * \brief Set the timer to the current time.
         */
        inline void Reset(){ start = std::chrono::high_resolution_clock::now(); }
        /**
         * \brief Returns the elapsed time.
         * \returns double
         */
        inline double Elapsed() const { return std::chrono::duration_cast<std::chrono::duration<double> >(std::chrono::high_resolution_clock::now() - start).count()*1000; }
    private:
        /// Initial time.
        std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();
        /// Elapsed time.
        double duration;
    };
}
#endif
