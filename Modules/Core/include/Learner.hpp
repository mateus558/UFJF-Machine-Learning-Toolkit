//
// Created by Mateus Coutinho Mari on 9/7/2018.
//

#ifndef LEARNER_HPP
#define LEARNER_HPP
#pragma once

#include "Solution.hpp"
#include "Data.hpp"
#include "Timer.hpp"

namespace mltk{

  template <typename T>
  class Learner;

  template < class T > using LearnerPointer = std::shared_ptr<mltk::Learner< T > >;

  template < typename T >
  class Learner {
  protected:
      /// Samples used in the model training.
      std::shared_ptr<Data< T > > samples;
      /// Learning rate
      double rate = 0.5f;
      /// Initial time.
      double start_time = 0.0f;
      /// Maximum time of training.
      double max_time = 110;
      /// Number of steps in the data.
      int steps = 0;
      /// Number of updates of the weights.
      int ctot = 0;
      /// Max precision.
      double EPS = 0.0000001;
      /// Minimun Increment
      double MIN_INC = 1.001;
      /// Max number of iterations.
      int MAX_IT = 1E9;
      /// Max number of updates.
      int MAX_UP = 1E9;
      int MAX_EPOCH = 1E9;
      /// Verbose level of the output.
      int verbose = 1;
      /// Timer used to measure the time elapsed in the execution of a Learner.
      Timer timer = Timer();

  public:
      Learner< T > (){}

      Learner< T > (DataPointer< T > _samples): samples(_samples) {}

      Learner< T > (const Learner< T > &learner){
        this->samples = learner.samples;
        this->EPS = learner.EPS;
        this->rate = learner.rate;
        this->start_time = learner.start_time;
        this->max_time = learner.max_time;
        this->steps = learner.steps;
        this->ctot = learner.ctot;
        this->MIN_INC = learner.MIN_INC;
        this->MAX_IT = learner.MAX_IT;
        this->MAX_UP = learner.MAX_UP;
        this->MAX_EPOCH = learner.MAX_EPOCH;
        this->verbose = learner.verbose;
      }


      /**
       * \brief Function that execute the training phase of a Learner.
       * \return void
       */
      virtual bool train () = 0;
      /**
       * \brief Returns the class of a feature point based on the trained Learner.
       * \param Point< T >  x (???) Features point to be evaluated.
       * \return int
       */
      virtual double evaluate (const Point< T > &p, bool raw_value=false) = 0;
      
      /*********************************************
       *               Getters                     *
       *********************************************/

      /**
       * \brief getFormulationString Returns a string that represents the formulation of the learner (Primal or Dual).
       * \return std::string
       */
      virtual std::string getFormulationString() = 0;

      /**
      * \brief Get the elapsed time in the training phase of the Learner.
      * \return double
      */
      inline double getElapsedTime() const { return timer.Elapsed(); }

      /**
      * \brief Get the total number of updates of the Learner.
      * \return int
      */
      inline int getCtot() const { return ctot; }
      /**
       * \brief getSteps Returns the number of steps through the data by the Learner.
       * \return int
       */
      inline int getSteps() const { return steps; }
      /**
       * \brief getUpdates Returns the number of updates needed to get to the the solution.
       * \return int
       */
      inline int getUpdates() const { return ctot; }
      /**
       * \brief getMaxTime Returns the maximum running time in the training phase of the Learner.
       * \return double
       */
      inline double getMaxTime() const { return max_time; }

      /*********************************************
      *               Setters                     *
      *********************************************/

      /**
       * \brief setSamples Set the samples used by the Learner.
       * \param samples Samples to be used.
       */
      virtual void setSamples(std::shared_ptr<Data< T > > samples) {this->samples = samples;}
      /**
       * \brief setTimer Set the timer used by the Learner.
       * \param timer Timer to be used.
       */
      void setTimer(Timer timer) { this->timer = timer; }
      /**
       * \brief Set the partial number of steps used in the training phase of the Learner.
       * \param steps Number of steps.
       */
      inline void setSteps(int steps) { this->steps = steps; }
      /**
        * \brief Set the partial number of updates of the Learner.
        * \param ctot Number of updates.
        */
      void setCtot(int ctot) {this->ctot = ctot;}
      /**
       * \brief Set the level of verbose.
       * \param verbose level of verbose.
       */
      void setVerbose(int verbose) {this->verbose = verbose;}
      /**
       * \brief setStartTime Set the initial time of the Learner.
       * \param start_time Initial time.
       */
      void setStartTime(double start_time) { this->start_time = start_time;}
      /**
       * \brief Set the max time of execution.
       * \param max_time  Max time.
       */
      void setMaxTime(double max_time) {this->max_time = max_time;}
      /**
       * \brief setEPS Set the precision of the Learner.
       * \param EPS Precision.
       */
      void setEPS(double EPS) {this->EPS = EPS;}
      /**
       * \brief setMaxIterations Set the max number of iterations of the Learner.
       * \param MAX_IT Number max of iterations.
       */
      void setMaxIterations(int MAX_IT) {this->MAX_IT = MAX_IT;}
      /**
       * \brief setMaxIterations Set the max number of updates of the Learner.
       * \param MAX_IT Number max of updates.
       */
      void setMaxUpdates(int MAX_UP) {this->MAX_UP = MAX_UP;}
      /**
       * \brief Set the learning rate of the Learner.
       * \param rate Learning rate.
       */
      void setLearningRate(double rate) {this->rate = rate;}
  };
}

#endif //UFJF_MLTK_LEARNER_H
