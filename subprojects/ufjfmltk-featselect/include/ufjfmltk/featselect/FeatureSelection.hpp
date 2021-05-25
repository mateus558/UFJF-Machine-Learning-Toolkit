//
// Created by Mateus Coutinho Marim on 6/26/2018.
//

#ifndef UFJF_MLTK_FEATURESELECTION_H
#define UFJF_MLTK_FEATURESELECTION_H
#pragma once

#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/valid/Validation.hpp"
#include <memory>

namespace mltk{
    /**
     * \brief Namespace for feature selection methods.
     */
    namespace featselect {
        template<typename T>
        class FeatureSelection {
            /// Attributes
        protected:
            /// Samples where the features will be selected
            std::shared_ptr<Data<T> > samples;
            /// Classifier used by the method
            classifier::Classifier<T> *classifier;
            /// Structure to hold the cross-validation result
            validation::CrossValidation *cv;
            /// Solution of the feature selection
            Solution solution;
            /// Timer for time measuring
            Timer timer;

            /// Max depth of search constant
            const int MAX_DEPTH = 99999;
            /// Max breadth of search constant
            const int MAX_BREATH = 99999;
            /// Error tolerance constant
            const double NUM_ERROR_EPS = 0.05;
            /// Depth of search
            int depth = 0;
            /// Final dimension
            int final_dim = 0;
            /// Jump size
            int jump = 0;
            /// Number of levels to be skipped
            int skip = 0;
            /// Leave one out
            bool leave_one_out = false;
            /// Verbose level
            int verbose = 0;

        public:

            /**
             * \brief Function that executes the feature selection phase.
             * \return void std::shared_ptr<Data< T > >
             */
            virtual Data<T> selectFeatures() = 0;

            /*********************************************
             *               Setters                     *
             *********************************************/

            /**
             * \brief setSamples Set the samples used for the FeatureSelection.
             * \param samples Samples to be used.
             */
            void setSamples(const std::shared_ptr<Data<T> > &samples) { this->samples = samples; }

            /**
             * \brief setClassifier Set the classifier used for the FeatureSelection.
             * \param classifier Reference to the classifier to be used.
             */
            void setClassifier(classifier::Classifier<T> *classifier) {
                this->classifier = classifier;
                this->classifier->setVerbose(0);
            }

            /**
             * \brief setFinalDimension Set the classifier used for the FeatureSelection.
             * \param final_dim The final number of dimensions.
             */
            void setFinalDimension(int final_dim) { this->final_dim = final_dim; }

            /**
             * \brief setDepth Set the depth of the search.
             * \param depth Depth of search.
             */
            void setDepth(int depth) { this->depth = depth; }

            /**
             * \brief setJump Set the jump value.
             * \param jump Jump value.
             */
            void setJump(int jump) { this->jump = jump; }

            /**
             * \brief setSkip Set the skip value.
             * \param skip Skip value.
             */
            void setSkip(int skip) { this->skip = skip; }

            /**
             * \brief setVerbose Set the verbose level.
             * \param verbose Verbose level.
             */
            void setVerbose(int verbose) { this->verbose = verbose; }

            /**
             * \brief setCrossValidation Set the cross validation structure.
             * \param cv Cross validation structure.
             */
            void setCrossValidation(validation::CrossValidation *cv) { this->cv = cv; }
        };
    }
}
#endif //UFJF_MLTK_FEATURESELECTION_H
