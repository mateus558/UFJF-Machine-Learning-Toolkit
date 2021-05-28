//
// Created by mateus558 on 29/07/19.
//

#ifndef UFJF_MLTK_ENSEMBLE_HPP
#define UFJF_MLTK_ENSEMBLE_HPP

#include <ufjfmltk/core/Learner.hpp>
#include <ufjfmltk/classifier/Classifier.hpp>

namespace mltk{
namespace ensemble{        
        /**
         * \brief Namespace for ensemble methods.
         */
        template <typename T>
        class Ensemble: virtual public Learner< T > {
            // Attributes
        protected:
            /// Committee size.
            size_t c_size{};
            /// Pointer to base learner used by the ensemble method.
            std::vector< LearnerPointer< T > > m_learners;
            /// Ensemble solution.
            Solution solution;
        public:
            Ensemble() = default;
            explicit Ensemble(DataPointer< T > samples): Learner< T > (samples) {}
            /*********************************************
             *               Getters                     *
             *********************************************/

            /**
             * \brief getSolution Returns the solution of the Ensemble method.
             * \return Solution
             */
            [[nodiscard]] Solution getSolution() const {return solution;}
            /**
             * \brief getSolution Returns a reference to the solution of the Ensemble method.
             * \return Solution
             */
            Solution *getSolutionRef() { return &solution; }

            /*********************************************
             *               Setters                     *
             *********************************************/

            /**
             * \brief setLearner Set the base learner to be used by the ensemble method. Make sure to set the base learner
             * parameters before setting it.
             * \return void
             */
            void setLearner(Learner<T> *learner) {
                Ensemble::learner = learner;
            }

            /**
             * \brief setCommitteeSize Set the learner committee size.
             * \return void
             */
            void setCommitteeSize(size_t c_size) {
                Ensemble::c_size = c_size;
            }

            size_t size(){
                return this->m_learners.size();
            }

            auto begin(){
                return this->m_learners.begin();
            }

            auto end(){
                return this->m_learners.end();
            }

            LearnerPointer<T> operator[](size_t idx) const  {
                return this->m_learners[idx];
            }

            auto learners(){
                return this->m_learners;
            }

        };
}
}

#endif //UFJF_MLTK_ENSEMBLE_HPP
