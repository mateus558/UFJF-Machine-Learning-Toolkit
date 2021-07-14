//
// Created by mateus on 29/05/2021.
//

#include "ufjfmltk/core/Datasets.hpp"

namespace mltk::datasets {
    mltk::Data<double> make_spirals(size_t n_samples, int n_classes, bool shuffle, double noise, size_t n_loops,
                                    double margin, size_t seed) {
        mltk::random::init(seed);
        size_t samples_byclass = std::ceil(n_samples / n_classes);
        mltk::Data<double> dataset;

        dataset.setName("spirals");

        mltk::Point<> theta = mltk::random_init<double>(samples_byclass, seed) * n_loops * 360.0 * M_PI / 180.0;
        mltk::Point<> r = theta * margin * n_classes;
        for (int i = 0; i < n_classes; i++) {
            mltk::Point<> rotated_theta = theta + i * 2.0 * M_PI / n_classes;
            mltk::Point<> x1 = r * mltk::cos(rotated_theta) + mltk::random_init<double>(samples_byclass, seed) * noise;
            mltk::Point<> x2 = r * mltk::sin(rotated_theta) + mltk::random_init<double>(samples_byclass, seed) * noise;

            for (size_t j = 0; j < samples_byclass; j++) {
                auto point = mltk::Point<double>({x1[j], x2[j]});
                if (n_classes == 2) point.Y() = (i == 0) ? -1 : 1;
                else if (n_classes > 0) point.Y() = i + 1;
                dataset.insertPoint(point);
            }
        }
        if (shuffle) dataset.shuffle(seed);

        return dataset;
    }

    BlobsPair make_blobs(size_t n_samples, int n_centers, int n_dims, double cluster_std, double center_min,
                         double center_max, bool shuffle, const bool has_classes, const size_t seed) {
        mltk::random::init(seed);
        std::vector<mltk::Point<>> centers(n_centers, mltk::Point<>(n_dims, 0.0));
        std::vector<size_t> samples_per_center(n_centers, std::floor(n_samples / n_centers));
        std::vector<double> clusters_std(n_centers, cluster_std);

        std::for_each(centers.begin(), centers.end(), [&center_min, &center_max](auto &center) {
            for (auto &x: center) {
                x = mltk::random::floatInRange<double>(center_min, center_max);
            }
        });

        return make_blobs(samples_per_center, centers, clusters_std, n_dims, shuffle, has_classes, seed);
    }

    BlobsPair make_blobs(const std::vector<size_t> &n_samples, const std::vector<mltk::Point<double>> &centers,
                         std::vector<double> clusters_std, int n_dims, bool shuffle, bool has_classes,
                         size_t seed) {
        mltk::random::init(seed);
        std::cout << seed << std::endl;
        mltk::Data<double> dataset;
        BlobsPair pair;
        dataset.setName("blobs");
        if(has_classes) dataset.setType("Classification");
        else dataset.setType("Clustering");

        for (const auto &n: n_samples) {
            for (int c = 0; c < centers.size(); c++) {
                auto center = centers[c];
                for (int i = 0; i < n; i++) {
                    mltk::Point<double> p(n_dims, 0.0);
                    for (int j = 0; j < n_dims; j++) {
                        p[j] = mltk::random::floatInRange<double, std::normal_distribution<double>>(center[j],
                                                                                                    clusters_std[c]);
                    }
                    if(has_classes) {
                        if (centers.size() == 2) {
                            p.Y() = (c == 0) ? -1 : 1;
                        } else {
                            p.Y() = c + 1;
                        }
                    }
                    dataset.insertPoint(p);
                }
            }
        }
        if (shuffle) dataset.shuffle(seed);
        pair.dataset = dataset;
        pair.centers = centers;
        return pair;
    }

    RegPair make_regression(size_t n_samples, size_t n_dims, double bias, double noise, double stdev,
                            size_t n_informative, bool shuffle, size_t seed) {
        mltk::random::init(seed);
        mltk::Data<double> dataset(n_samples, n_dims);
        RegPair pair;
        mltk::Point<double> ground_truth(n_dims, 0.0), y(n_samples);
        n_informative = std::min(n_dims, n_informative);
        auto random_init = [&n_dims, seed](const mltk::PointPointer<double> &point) {
            *point = mltk::random_init<double, std::normal_distribution<double>>(0, 1, n_dims, seed);
        };

        dataset.apply(random_init);

        for (size_t i = 0; i < n_informative; i++) {
            ground_truth[i] = mltk::random::floatInRange<double, std::normal_distribution<double>>(0, 1);
        }

        auto generate_values = [bias, &ground_truth, noise, stdev](mltk::PointPointer<double> p) {
            if (noise > 0) {
                p->Y() = mltk::dot(*p, ground_truth) + bias +
                         mltk::random::floatInRange<double, std::normal_distribution<double> >(stdev, noise);
            } else {
                p->Y() = mltk::dot(*p, ground_truth) + bias;
            }
        };

        dataset.apply(generate_values);

        if (shuffle) {
            dataset.shuffle(seed);
        }
        dataset.setName("artificial regression");
        dataset.setType("Regression");
        pair.dataset = dataset;
        pair.coef = ground_truth;
        return pair;
    }
}