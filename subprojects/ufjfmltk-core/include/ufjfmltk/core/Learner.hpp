//
// Created by Mateus Coutinho Mari on 9/7/2018.
//

#pragma once
#ifndef LEARNER_HPP
#define LEARNER_HPP

#include "Solution.hpp"
#include "Data.hpp"
#include "Timer.hpp"

namespace mltk{

    template <typename T>
    class Learner;
    
    template < class T > using LearnerPointer = std::shared_ptr<mltk::Learner< T > >;
    
    template < typename T >
    class  Learner {
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
      /// seed for random operations.
      size_t seed = 0;
      double pred_prob = 1.0;
    
    public:
      virtual ~Learner() = default;
    
      Learner< T >() = default;
    
      explicit Learner< T > (DataPointer< T > _samples): samples(_samples) {}
    
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
        this->seed = learner.seed;
      }
    
    
      /**
       * \brief Function that execute the training phase of a Learner.
       * \return void
       */
      virtual bool train () = 0;
      /**
       * \brief Returns the class of a feature point based on the trained Learner.
       * \param p Point to be evaluated.
       * \return Prediction made by the learner.
       */
      virtual double evaluate (const Point< T > &p, bool raw_value=false) = 0;
    
      /**
       * @brief evaluate a batch of points.
       * @param data dataset containing points for evaluation.
       * @return copy of the passed data already evaluated.
       */
      virtual Data<T> batchEvaluate (const Data< T >& data);
      
      /*********************************************
       *               Getters                     *
       *********************************************/
    
      /**
       * \brief getFormulationString Returns a string that represents the formulation of the learner (Primal or Dual).
       * \return std::string
       */
      virtual std::string getFormulationString() = 0;
      /**
       * @brief Get the Data used by the learner.
       * @return Data object.
       */
      inline auto getSamples() { return this->samples; }
      /**
      * \brief Get the elapsed time in the training phase of the Learner.
      * \return double
      */
      [[nodiscard]] inline double getElapsedTime() const { return timer.elapsed(); }
      /**
      * \brief Get the total number of updates of the Learner.
      * \return int
      */
      [[nodiscard]] inline int getCtot() const { return ctot; }
      /**
       * \brief getSteps Returns the number of steps through the data by the Learner.
       * \return int
       */
      [[nodiscard]] inline int getSteps() const { return steps; }
      /**
       * \brief getUpdates Returns the number of updates needed to get to the the solution.
       * \return int
       */
      [[nodiscard]] inline int getUpdates() const { return ctot; }
      /**
       * \brief getMaxTime Returns the maximum running time in the training phase of the Learner.
       * \return double
       */
      [[nodiscard]] inline double getMaxTime() const { return max_time; }
      /**
       * @brief Get the probability of the last prediction.
       * @return Last prediction probability.
       */
      [[nodiscard]] double getPredictionProbability() const { return pred_prob; }
    
      /*********************************************
      *               Setters                     *
      *********************************************/
    
      /**
       * @brief Set the seed to be used by the learner.
       * @param _seed Seed to be used.
       */
      void setSeed(const size_t _seed){ this->seed = _seed; }
      /**
       * \brief setSamples Set the samples used by the Learner.
       * \param data Samples to be used.
       */
      virtual void setSamples(const Data< T > &data) { this->samples = make_data<T>(data); }
      /**
       * \brief setSamples Set the samples used by the Learner.
       * \param data Samples to be used.
       */
      virtual void setSamples(DataPointer< T > data) { this->samples = data; }
      /**
       * \brief setTimer Set the timer used by the Learner.
       * \param _timer Timer to be used.
       */
      void setTimer(Timer _timer) { this->timer = _timer; }
      /**
       * \brief Set the partial number of steps used in the training phase of the Learner.
       * \param _steps Number of steps.
       */
      inline void setSteps(int _steps) { this->steps = _steps; }
      /**
        * \brief Set the partial number of updates of the Learner.
        * \param _ctot Number of updates.
        */
      void setCtot(int _ctot) { this->ctot = _ctot;}
      /**
       * \brief Set the level of verbose.
       * \param _verbose level of verbose.
       */
      void setVerbose(int _verbose) { this->verbose = _verbose;}
      /**
       * \brief setStartTime Set the initial time of the Learner.
       * \param stime Initial time.
       */
      void setStartTime(double stime) { this->start_time = stime;}
      /**
       * \brief Set the max time of execution.
       * \param maxtime  Max time.
       */
      void setMaxTime(double maxtime) { this->max_time = maxtime;}
      /**
       * \brief setEPS Set the precision of the Learner.
       * \param eps Precision.
       */
      void setEPS(double eps) { this->EPS = eps;}
      /**
       * \brief setMaxIterations Set the max number of iterations of the Learner.
       * \param max_it Number max of iterations.
       */
      void setMaxIterations(int max_it) { this->MAX_IT = max_it;}
      /**
       * @brief Set the max number of epochs for the learner training.
       * @param MAX_EPOCHS Max number of epochs.
       */
      void setMaxEpochs(int MAX_EPOCHS) {this->MAX_EPOCH = MAX_EPOCHS;}
      /**
       * \brief setMaxIterations Set the max number of updates of the Learner.
       * \param MAX_IT Number max of updates.
       */
      void setMaxUpdates(int max_up) { this->MAX_UP = max_up;}
      /**
       * \brief Set the learning rate of the Learner.
       * \param learning_rate Learning rate.
       */
      void setLearningRate(double learning_rate) { this->rate = learning_rate;}
    };

    /*********************************************
     *               Implementation              *
     *********************************************/

    template<typename T>
    Data<T> Learner<T>::batchEvaluate (const Data< T >& data){
        Data<T> result;
        std::for_each(data.begin(), data.end(), [&](const mltk::PointPointer<T> p){
            mltk::Point<T> q(*p);
    
            q.Y() = this->evaluate(q, false);
            result.insertPoint(q);
        });
        return result;
    }
}

#endif //UFJF_MLTK_LEARNER_H
