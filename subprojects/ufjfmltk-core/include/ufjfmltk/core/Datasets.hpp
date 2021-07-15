//
// Created by mateus on 20/05/2021.
//

#ifndef UFJFMLTK_DATASETS_HPP
#define UFJFMLTK_DATASETS_HPP
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "ufjfmltk/core/Data.hpp"

/**
* \brief Namespace for artificial datasets generation.
*/
namespace mltk::datasets{
    using Centers = std::vector<mltk::Point<double>>;

    struct RegPair{
        /// Regression dataset
        mltk::Data<double> dataset;
        /// true coefficients
        mltk::Point<double> coef;
    };

    struct BlobsPair{
        /// blobs dataset
        mltk::Data<double> dataset;
        /// centers used for points clouds generation
        Centers centers;
    };

    /*
     * \brief generates a synthetic data set composed of interlaced Archimedean spirals [<a href="https://github.com/KNODIS-Research-Group/make_spirals">source</a>].
     * \param n_samples number of samples on the dataset.
     * \param n_classes number of classes or spirals.
     * \param shuffle tells if the dataset must be shuffled after generation.
     * \param noise value of the noise added to the dataset.
     * \param n_loops how many loops each spiral must have.
     * \param margin margin between spirals.
     * \param seed seed for random data generation.
     * \return spirals artificial dataset.
     */
    mltk::Data<double> make_spirals(size_t n_samples=100, int n_classes=2, bool shuffle=true, double noise=1.0,
                            size_t n_loops=2, double margin = 0.5, size_t seed = 0);

    /*
     * \brief Generate isotropic Gaussian blobs for clustering or classification [<a href="https://scikit-learn.org/stable/modules/generated/sklearn.datasets.make_blobs.html">source</a>].
     * \param n_samples number of samples on each gaussian blob.
     * \param n_centers number of classes or gaussian blobs.
     * \param n_dims dimensionality of the data.
     * \param cluster_std standard deviation for blobs generation.
     * \param center_min min value of generated data.
     * \param center_max max value of generated data.
     * \param shuffle tells if the dataset must be shuffled after generation.
     * \param has_classes tells if the returned data have labels for the blobs.
     * \param seed seed for random data generation.
     * \return pair containing the dataset and blobs centers.
     */
    BlobsPair make_blobs(size_t n_samples=100, int n_centers=2, int n_dims=2, double cluster_std=1.0,
                                  double center_min=-10.0, double center_max=10.0, bool shuffle=true,
                                  bool has_classes=true, size_t seed = 0);

    /*
     * \brief Generate isotropic Gaussian blobs for clustering or classification from given centers and samples distribution.
     * \param n_samples number of samples in each gaussian blob.
     * \param centers centers of the gaussian blobs.
     * \param clusters_std vector of standard deviations for each blob.
     * \param n_dims dimensionality of the data.
     * \param shuffle tells if the dataset must be shuffled after generation.
     * \param has_classes tells if the returned data have labels for the blobs.
     * \param seed seed for random data generation.
     * \return pair containing the dataset and blobs centers.
     */
    BlobsPair make_blobs(const std::vector<size_t>& n_samples, const std::vector<mltk::Point<double>>& centers,
                         std::vector<double> clusters_std, int n_dims=2, bool shuffle=true,
                         bool has_classes=true, size_t seed = 0);

    /*
     * \brief Generate a random regression problem [<a href="https://scikit-learn.org/stable/modules/generated/sklearn.datasets.make_regression.html">source</a>].
     * \param n_samples number of samples on the dataset.
     * \param n_dims dimensionality of the data.
     * \param bias The bias term in the underlying linear model.
     * \param noise The standard deviation of the gaussian noise applied to the output.
     * \param stdev The standard deviation of the gaussian noise applied to the output.
     * \param n_informative The number of informative features, i.e., the number of features used to build the linear model used to generate the output.
     * \param shuffle Shuffle the samples and the features.
     * \param seed seed for random data generation.
     * \return A struct containing the coefficients and the generated regression dataset.
     */
    RegPair make_regression(size_t n_samples=100, size_t n_dims=100, double bias=0.0, double noise=0.1, double stdev=0.01,
                            size_t n_informative=10, bool shuffle=true, size_t seed=0);

}

#endif //UFJFMLTK_DATASETS_HPP
