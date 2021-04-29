//
// Created by mateus on 29/04/2021.
//

#include <gtest/gtest.h>
#include "ufjfmltk/Classifier.hpp"
#include "ufjfmltk/valid/Validation.hpp"

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
    }
    mltk::Data<double> mult, bin;
};

TEST_F(ClassifierTest, BinClassifierTest){
    mltk::classifier::KNNClassifier<double> knn(bin, 3);
    mltk::classifier::IMAp<double> ima(bin);
    mltk::classifier::PerceptronPrimal<double> perc(bin);
    ima.setVerbose(0);
    ASSERT_GT(mltk::validation::kkfold(bin, knn, 10, 10, 0).accuracy, 95);
    ASSERT_GT(100-mltk::validation::kfold(bin, knn, 10, 10, 0), 95);
    ASSERT_GT(100-mltk::validation::kfold(bin, ima, 10, 10, 0), 95);
    ASSERT_GT(100-mltk::validation::kfold(bin, perc, 10, 10, 0), 95);
}

TEST_F(ClassifierTest, MultiClassifierTest){
    mltk::classifier::KNNClassifier<double> knn(mult, 3);
    mltk::classifier::IMAp<double> ima(mult);
    mltk::classifier::PerceptronPrimal<double> perc;
    ima.setVerbose(0);
    mltk::classifier::OneVsAll<double> ova(ima);
    mltk::classifier::OneVsOne<double> ovo(mult, perc);

    ASSERT_GT(mltk::validation::kkfold(mult, knn, 10, 10, 0).accuracy, 95);
    ASSERT_GT(100-mltk::validation::kfold(mult, knn, 10, 10, 0), 95);
    ASSERT_GT(100-mltk::validation::kfold(mult, ova, 10, 10, 0), 95);
    ASSERT_GT(100-mltk::validation::kfold(mult, ovo, 10, 10, 0), 90);
}
