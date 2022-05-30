/*! Classifier base class.
   \file Classifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef CLASSIFIER__HPP
#define CLASSIFIER__HPP


#include "ufjfmltk/core/Learner.hpp"
namespace mltk{
    /**
     * \brief Namespace for classification methods.
     */
    namespace classifier {
        template<typename T>
        class Classifier : virtual public Learner<T> {
            // Attributes
        protected :
            /// Inform if there's an initial solution.
            bool hasInitialSolution = false;
            /// Support vectors points.
            std::vector<Point<T> > svs;
            /// Classifier solution.
            Solution solution;
            /// Classifier margin.
            double gamma = 0;

            // Operations
        public :

            Classifier<T>() {}

            Classifier<T>(DataPointer<T> samples) : Learner<T>(samples) {}

            Classifier<T>(const Classifier<T> &classifier)
                    : Learner<T>(classifier) {
                this->hasInitialSolution = classifier.hasInitialSolution;
                this->gamma = classifier.gamma;
            }

            virtual double evaluateProbability(const mltk::Point<double>& p){return 0.0;}
            mltk::Point<int> batchEvaluateProbability(const mltk::Data<T>& data);
            /*********************************************
             *               Getters                     *
             *********************************************/

            /**
             * \brief getSolution Returns the solution of the classifier.
             * \return Solution
             */
            Solution getSolution() const { return solution; }

            /**
             * \brief getSolution Returns a reference to the solution of the classifier.
             * \return Solution
             */
            Solution *getSolutionRef() { return &solution; }

            /*********************************************
             *               Setters                     *
             *********************************************/
            /**
            * \brief Set the gamma (margin) of the classifier.
            * \param gamma Gamma (margin) of the classifier.
            */
            inline void setGamma(double gamma) { this->gamma = gamma; }

            /**
            * \brief setW Set the weights vector of the classifier.
            * \param w weights vector.
            */
            void setW(std::vector<double> w) { this->solution.w = w; }

            /**
             * \brief setSolution Set a solution for the classifier.
             * \param solution Solution to be set.
             */
            void setSolution(Solution solution) { this->solution = solution; }
        };

        template<typename T>
        mltk::Point<int> Classifier<T>::batchEvaluateProbability(const Data <T> &data) {
            mltk::Point<int> preds(data.size());
            std::transform(data.begin(), data.end(), preds.begin(), [this](auto point){
                return this->evaluateProbability(*point);
            });
            return preds;
        }
    }
}

#endif
