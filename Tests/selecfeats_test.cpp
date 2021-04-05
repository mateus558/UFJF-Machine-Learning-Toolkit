//
// Created by mateuscmarim on 25/01/2021.
//

#include "Data.hpp"

int main(){
    mltk::Data<double> data("iris.data");
    std::vector<size_t> feats = {0,2,3};
    auto feat_data = data.selectFeatures(feats);
    assert(feats.size() == feat_data.dim());
    assert(data.size() == feats_data.size());
    for(auto const& point: data){
        for(auto const& feat_point: feat_data){
            for(auto feat: feats){
                assert((*point)[feat] == (*feat_point)[feat]);
            }
            assert(point->Y() == feat_point->Y());
            assert(point->Id() == feat_point->Id());
            assert(point->Alpha() == feat_point->Alpha());
        }
    }
}