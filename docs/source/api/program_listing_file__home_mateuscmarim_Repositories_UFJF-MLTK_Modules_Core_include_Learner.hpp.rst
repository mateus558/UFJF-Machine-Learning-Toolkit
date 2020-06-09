
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Learner.hpp:

Program Listing for File Learner.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Learner.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Learner.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by Mateus Coutinho Mari on 9/7/2018.
   //
   
   #ifndef UFJF_MLTK_LEARNER_H
   #define UFJF_MLTK_LEARNER_H
   
   #include "Solution.hpp"
   #include "Data.hpp"
   #include "Timer.hpp"
   
   template < typename T >
   class Learner {
   protected:
       std::shared_ptr<Data< T > > samples;
       double rate = 0.5f;
       double start_time = 0.0f;
       double max_time = 110;
       int steps = 0;
       int ctot = 0;
       double EPS = 0.0000001;
       double MIN_INC = 1.001;
       int MAX_IT = 1E9;
       int MAX_UP = 1E9;
       int MAX_EPOCH = 9999;
       int verbose = 1;
       Timer timer = Timer();
   
   public:
       virtual bool train () = 0;
       virtual double evaluate (Point< T > p) = 0;
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       virtual std::string getFormulationString() = 0;
   
       inline double getElapsedTime() const { return timer.Elapsed(); }
   
       inline int getCtot() const { return ctot; }
       inline int getSteps() const { return steps; }
       inline int getUpdates() const { return ctot; }
       inline double getMaxTime() const { return max_time; }
   
       /*********************************************
       *               Setters                     *
       *********************************************/
   
       virtual void setSamples(std::shared_ptr<Data< T > > samples) {this->samples = samples;}
       void setTimer(Timer timer) { this->timer = timer; }
       inline void setSteps(int steps) { this->steps = steps; }
       void setCtot(int ctot) {this->ctot = ctot;}
       void setVerbose(int verbose) {this->verbose = verbose;}
       void setStartTime(double start_time) { this->start_time = start_time;}
       void setMaxTime(double max_time) {this->max_time = max_time;}
       void setEPS(double EPS) {this->EPS = EPS;}
       void setMaxIterations(int MAX_IT) {this->MAX_IT = MAX_IT;}
       void setMaxUpdates(int MAX_UP) {this->MAX_UP = MAX_UP;}
       void setLearningRate(double rate) {this->rate = rate;}
   };
   
   
   #endif //UFJF_MLTK_LEARNER_H
