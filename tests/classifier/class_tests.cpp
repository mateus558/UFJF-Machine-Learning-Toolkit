//
// Created by mateus on 29/04/2021.
//

#include <gtest/gtest.h>
#include "ufjfmltk/Classifier.hpp"
#include "ufjfmltk/Validation.hpp"
#include "ufjfmltk/core/Utils.hpp"

class ClassifierTest: public ::testing::Test
{
protected:
    void SetUp() override{
        if(!mult.load("iris_mult.csv")){
            std::cerr << "Error loading multiclass dataset." << std::endl;
        }
        if(!bin.load("iris.data")){
            std::cerr << "Error loading binary dataset." << std::endl;
        }
        if(!wine.load("wine.data")){
            std::cerr << "Error loading binary dataset." << std::endl;
        }
    }
    mltk::Data<double> mult, bin, wine;
};

TEST_F(ClassifierTest, BinClassifierTest){
    mltk::classifier::KNNClassifier<double> knn(bin, 3);
    mltk::classifier::IMAp<double> ima(bin);
    mltk::classifier::IMAp<double> ima1(bin, -1);
    mltk::classifier::IMAp<double> ima3(bin, 3);
    mltk::classifier::IMAp<double> imainf(bin, 1);
    mltk::classifier::IMApFixedMargin<double> ima_fixed(mltk::make_data<double>(bin), 0.5);
    mltk::classifier::PerceptronPrimal<double> perc(bin);
    mltk::classifier::BalancedPerceptron<double> bperc(bin);

    ima1.setVerbose(0);
    imainf.setVerbose(0);
    ima3.setVerbose(0);

    ASSERT_GT(mltk::validation::kkfold(bin, knn, 10, 10, 2).accuracy, 95);
    ASSERT_GT(mltk::validation::kfold(bin, knn, 10, 10, 2).accuracy, 95);
    ASSERT_GT(mltk::validation::kfold(bin, ima, 10, 10, 0).accuracy, 90);
    ASSERT_GT(mltk::validation::kfold(bin, ima1, 10, 10, 0).accuracy, 90);
    ASSERT_GT(mltk::validation::kfold(bin, imainf, 10, 10, 0).accuracy, 90);
    ASSERT_GT(mltk::validation::kfold(bin, ima3, 10, 10, 0).accuracy, 90);
    ASSERT_GT(mltk::validation::kfold(bin, ima_fixed, 10, 10, 0).accuracy, 30);
    ASSERT_GT(mltk::validation::kfold(bin, perc, 10, 10, 0).accuracy, 95);
    ASSERT_GT(mltk::validation::kfold(bin, bperc, 10, 10, 0).accuracy, 90);
}

TEST_F(ClassifierTest, MultiClassifierTest){
    mltk::classifier::KNNClassifier<double> knn(mult, 3);
    mltk::classifier::IMAp<double> ima(mult);
    mltk::classifier::PerceptronPrimal<double> perc;
    mltk::classifier::OneVsAll<double> ova(ima);
    mltk::classifier::OneVsOne<double> ovo(mult, perc);

    ima.setVerbose(0);

    ASSERT_GT(mltk::validation::kkfold(mult, knn, 10, 10, 0).accuracy, 95);
    ASSERT_GT(mltk::validation::kfold(mult, knn, 10, 10, 0).accuracy, 95);
    ASSERT_GT(mltk::validation::kfold(mult, ova, 10, 10, 0).accuracy, 85);
    ASSERT_GT(mltk::validation::kfold(mult, ovo, 10, 10, 0).accuracy, 90);

    auto conf_mat = mltk::validation::generateConfusionMatrix(mult, ovo);
    mltk::utils::printConfusionMatrix(mult.classes(), mult.classesNames(), conf_mat);
    ASSERT_GT(mltk::validation::confusionMatrixAccuracy(conf_mat), 90);
}

TEST_F(ClassifierTest, SMOClassifier){
    mltk::classifier::SMO<double> smo_gauss(wine, "gaussian", 0, 0.5);
    mltk::classifier::SMO<double> smo_inner(wine, "inner_product", 0, 1);
    mltk::classifier::SMO<double> smo_poly(wine, "poly", 1, 3);
    double acc = 0;
    std::cout << "Testing with gaussian kernel" << std::endl;
    ASSERT_GT(mltk::validation::kfold(wine, smo_gauss, 10, 0).accuracy, 95);
    std::cout << "Testing with inner product kernel" << std::endl;
    smo_inner.setMaxEpochs(100);
    acc = mltk::validation::kfold(wine, smo_inner, 10, 0).accuracy;
    ASSERT_GE(acc, 74);
    std::cout << "Testing with polynomial kernel" << std::endl;
    smo_poly.setMaxEpochs(100);
    acc = mltk::validation::kfold(wine, smo_poly, 10, 0).accuracy;
    ASSERT_GT(acc, 60);
}

TEST_F(ClassifierTest, DualClassifier){
    mltk::classifier::PerceptronDual<double> perc_dual(bin, 0.5);
    mltk::classifier::PerceptronDual<double> perc_dual_gaussian(bin,mltk::KernelType::GAUSSIAN,
                                                                0.5);
    mltk::classifier::IMADual<double> ima_dual(bin);
    mltk::classifier::IMADual<double> ima_dual_gaussian(bin, mltk::KernelType::GAUSSIAN, 0.5);

    ima_dual_gaussian.setVerbose(3);
    ima_dual.setVerbose(0);
    std::cout << "Testing perceptron dual." << std::endl;
    ASSERT_GT(mltk::validation::kfold(bin, perc_dual, 10, 10, 0).accuracy, 40);
    ASSERT_GT(mltk::validation::kfold(bin, perc_dual_gaussian, 10, 10, 0).accuracy, 95);
    std::cout << "Testing IMA dual with 10-fold." << std::endl;
    ASSERT_GT(mltk::validation::kfold(bin, ima_dual, 10, 10, 0).accuracy, 40);
    std::cout << "Testing IMA dual with 10-10-fold." << std::endl;
    ASSERT_GT(mltk::validation::kkfold(bin, ima_dual, 10, 10, 0, 0).accuracy, 40);
    ASSERT_GT(mltk::validation::kfold(bin, ima_dual_gaussian, 10, 0, 0).accuracy, 95);

    mltk::utils::printConfusionMatrix(bin.classes(), bin.classesNames(), mltk::validation::generateConfusionMatrix(bin, perc_dual));
    mltk::utils::printConfusionMatrix(bin.classes(), bin.classesNames(), mltk::validation::generateConfusionMatrix(bin, ima_dual));
    mltk::utils::printConfusionMatrix(bin.classes(), bin.classesNames(), mltk::validation::generateConfusionMatrix(bin, ima_dual_gaussian));
}