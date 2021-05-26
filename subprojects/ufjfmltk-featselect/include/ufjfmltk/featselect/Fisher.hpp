//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#ifndef UFJF_MLTK_FISHER_H
#define UFJF_MLTK_FISHER_H
#pragma once

#include "FeatureSelection.hpp"

namespace mltk{
    namespace featselect {
        template<typename T = double>
        class Fisher : public FeatureSelection<T> {
        private:
            int number = 0;

            struct fisher_select_score {
                int fname;
                double score;
            };

            static int fisher_select_compare_score_greater(const fisher_select_score &a, const fisher_select_score &b);

        public:
            Fisher() = default;
            explicit Fisher(const Data<T>& samples = nullptr,
                            classifier::Classifier<T> *classifier = nullptr, int number = 0);

            Data<T> selectFeatures() override;
        };
    }
}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "../../../src/Fisher.cpp"
#endif
#endif //UFJF_MLTK_FISHER_H
