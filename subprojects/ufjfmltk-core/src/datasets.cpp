//
// Created by mateus on 20/05/2021.
//

#include "ufjfmltk/core/Datasets.hpp"

namespace mltk::datasets {
    mltk::Data<double> make_spirals(size_t n_samples, int n_classes, bool shuffle, double noise, size_t n_loops,
                               double margin, size_t seed) {
        mltk::random::init(seed);
        size_t samples_byclass = std::ceil(n_samples/n_classes);
        mltk::Data<double> dataset;
        mltk::Point<double> theta, r;

        dataset.setName("spirals");

        theta = mltk::random_init<double>(samples_byclass, seed) * n_loops * 360.0 * M_PI / 180.0;
        r = theta * margin * n_classes;

        for(int i = 0; i < n_classes; i++){
            mltk::Point<double> rotated_theta;
            mltk::Point<double> x1, x2;

            rotated_theta = theta + i * 2.0 * M_PI / n_classes;
            x1 = r * mltk::cos(rotated_theta) + mltk::random_init<double>(samples_byclass, seed) * noise;
            x2 = r * mltk::sin(rotated_theta) + mltk::random_init<double>(samples_byclass, seed) * noise;

            for(size_t j = 0; j < samples_byclass; j++){
                auto point = mltk::Point<double>({x1[j], x2[j]});
                point.Y() = i+1;
                dataset.insertPoint(point);
            }
        }
        if(shuffle) dataset.shuffle(seed);

        return dataset;
    }

    BlobsPair make_blobs(size_t n_samples, int n_centers, int n_dims, double cluster_std, double center_min,
                                  double center_max, bool shuffle, size_t seed){
        mltk::random::init(seed);
        std::vector<mltk::Point<>> centers(n_centers, mltk::Point<>(n_dims, 0.0));
        std::vector<size_t> samples_per_center(n_centers, std::floor(n_samples/n_centers));
        std::vector<double> clusters_std(n_centers, cluster_std);

        std::for_each(centers.begin(), centers.end(), [&center_min, &center_max](auto& center){
            for(auto& x: center){
                x = mltk::random::floatInRange<double>(center_min, center_max);
            }
        });

        return make_blobs(samples_per_center, centers, clusters_std, n_dims, shuffle, seed);
    }

    BlobsPair make_blobs(const std::vector<size_t>& n_samples, const std::vector<mltk::Point<double>>& centers,
                                  std::vector<double> clusters_std, int n_dims, bool shuffle, size_t seed){
        mltk::random::init(seed);
        mltk::Data<double> dataset;

        dataset.setName("blobs");
        dataset.setType("Classification");

        for(const auto& n: n_samples){
            for(size_t c = 0; c < centers.size(); c++){
                auto center = centers[c];
                for(int i = 0; i < n; i++){
                    mltk::Point<double> p(n_dims, 0.0);
                    for(int j = 0; j < n_dims; j++){
                        p[j] = mltk::random::floatInRange<double, double, std::normal_distribution<double>>(center[j], clusters_std[c]);
                    }
                    p.Y() = (double)c+1;
                    dataset.insertPoint(p);
                }
            }
        }
        if(shuffle) dataset.shuffle(seed);

        return std::make_pair(dataset, centers);
    }
}