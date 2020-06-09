
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Timer.hpp:

Program Listing for File Timer.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Timer.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Timer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef TIMER_H_INCLUDED
   #define TIMER_H_INCLUDED
   
   #include <cstdio>
   #include <ctime>
   
   class Timer {
   public:
       inline Timer() {start = std::clock();}
       inline void Reset(){ start = std::clock(); }
       inline double Elapsed() const { return 100.0f*( std::clock() - start ) / (double) CLOCKS_PER_SEC; }
   private:
       std::clock_t start;
       double duration;
   };
   #endif
