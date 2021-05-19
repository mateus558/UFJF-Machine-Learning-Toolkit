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
    private:
        using Clock = std::chrono::high_resolution_clock;
        typename Clock::time_point start_point;
    public:
        /**
         * \brief Contructor already initiate the timer to the current time.
         */
        inline Timer() : start_point(Clock::now()) {}
        /**
         * \brief Set the timer to the current time.
         */
        inline void reset(){ start_point = Clock::now(); }
        /**
         * \brief Returns the elapsed time.
         * \returns double
         */
        template <typename Rep = double, typename Units = typename std::chrono::milliseconds>
        inline double elapsed() const {
            std::atomic_thread_fence(std::memory_order_relaxed);
            auto counted_time = std::chrono::duration_cast<Units>(Clock::now() - start_point).count();
            std::atomic_thread_fence(std::memory_order_relaxed);
            return static_cast<Rep>(counted_time);
        }
    };
}
#endif
