//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#ifndef UFJF_MLTK_FISHER_H
#define UFJF_MLTK_FISHER_H

#include "FeatureSelection.hpp"

template < typename T >
class Fisher: public FeatureSelection< T > {
private:
    int number = 0;

    struct fisher_select_score
    {
        int fname;
        double score;
    };
    static int fisher_select_compare_score_greater(const fisher_select_score &a, const fisher_select_score &b);
public:
    Fisher(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T > *classifier = nullptr, int number = 0);
    std::shared_ptr<Data< T > > selectFeatures() override;
};

#endif //UFJF_MLTK_FISHER_H
