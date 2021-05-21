//
// Created by mateus on 20/05/2021.
//

#ifndef UFJFMLTK_DATASETS_HPP
#define UFJFMLTK_DATASETS_HPP

#include "ufjfmltk/core/Data.hpp"

namespace mltk::datasets{
    using Centers = std::vector<mltk::Point<double>>;
    using RegPair = std::pair<mltk::Data<double>, std::vector<double>>;

    struct BlobsPair{
        mltk::Data<double> dataset;
        Centers centers;
    };

    mltk::Data<double> make_spirals(size_t n_samples=100, int n_classes=2, bool shuffle=true, double noise=1.0,
                            size_t n_loops=2, double margin = 0.5, size_t seed = 0);

    BlobsPair make_blobs(size_t n_samples=100, int n_centers=2, int n_dims=2, double cluster_std=1.0,
                                  double center_min=-10.0, double center_max=10.0, bool shuffle=true,
                                  bool has_classes=true, size_t seed = 0);

    BlobsPair make_blobs(const std::vector<size_t>& n_samples, const std::vector<mltk::Point<double>>& centers,
                                  std::vector<double> clusters_std, int n_dims=2, bool shuffle=true,
                                  bool has_classes=true, size_t seed = 0);
}

#endif //UFJFMLTK_DATASETS_HPP
