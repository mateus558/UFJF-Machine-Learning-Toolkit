//
// Created by Mateus Coutinho Mari on 6/26/2018.
//

#ifndef UFJF_MLTK_RFE_H
#define UFJF_MLTK_RFE_H
#pragma once

#include "FeatureSelection.hpp"

namespace mltk{
    namespace featselect {
        template<typename T>
        class RFE : public FeatureSelection<T> {
        private:
            struct select_weight {
                double w;
                double val;
                double fname;
            };

            static int compare_weight_greater(const select_weight &a, const select_weight &b);

        public:
            explicit RFE(std::shared_ptr<Data<T> > samples = nullptr, classifier::Classifier<T> *classifier = nullptr,
                         validation::CrossValidation *cv = nullptr, int depth = 0, int skip = 0, int jump = 0,
                         bool leave_one_out = false);

            std::shared_ptr<Data<T> > selectFeatures() override;
        };
    }
}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../../../src/RFE.cpp"
#endif
#endif //UFJF_MLTK_RFE_H
