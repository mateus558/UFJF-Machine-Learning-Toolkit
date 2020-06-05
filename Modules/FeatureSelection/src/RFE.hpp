//
// Created by Mateus Coutinho Mari on 6/26/2018.
//

#ifndef UFJF_MLTK_RFE_H
#define UFJF_MLTK_RFE_H

#include "FeatureSelection.hpp"

template < typename T >
class RFE : public FeatureSelection< T > {
private:
    struct select_weight{
        double w;
        double val;
        double fname;
    };
    static int compare_weight_greater(const select_weight &a, const select_weight &b);
public:
    RFE(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T >* classifier = nullptr, typename Validation< T >::CrossValidation *cv = nullptr, int depth = 0, int skip = 0, int jump = 0, bool leave_one_out = false);
    std::shared_ptr<Data< T > > selectFeatures() override ;
};

#endif //UFJF_MLTK_RFE_H
