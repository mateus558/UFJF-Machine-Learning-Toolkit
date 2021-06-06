//
// Created by mateus on 16/05/2021.
//
#include <gtest/gtest.h>
#include "ufjfmltk/Classifier.hpp"
#include "ufjfmltk/Validation.hpp"
#include "ufjfmltk/core/Utils.hpp"
#include "ufjfmltk/Ensemble.hpp"

class EnsembleTest: public ::testing::Test
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

TEST_F(EnsembleTest, AdaBoostTest){
    mltk::classifier::BalancedPerceptron<double> bp;
    mltk::ensemble::AdaBoostClassifier<double> ada(bin, bp, 10);

    ASSERT_GT(mltk::validation::kfold(bin, ada, 10, 0).accuracy, 96);
}

TEST_F(EnsembleTest, PercComitteeTest){
    mltk::ensemble::PerceptronCommittee<double> perc_com(bin, 100);

    ASSERT_GT(mltk::validation::kfold(bin, perc_com, 10, 0).accuracy, 99);
}

TEST_F(EnsembleTest, BaggingTest){
    mltk::classifier::BalancedPerceptron<double> bp;
    mltk::ensemble::BaggingClassifier<double> bag(wine, bp, 100);
    ASSERT_GT(mltk::validation::kfold(wine, bag, 10, 0).accuracy, 72);
}

TEST_F(EnsembleTest, VotingTest){
    mltk::classifier::BalancedPerceptron<double> bp;
    mltk::classifier::IMAp<double> imap;
    mltk::classifier::KNNClassifier<double> knn(3);
    //mltk::classifier::IMADual<double> ima_dual(mltk::KernelType::GAUSSIAN, 0.5);
    //ima_dual.setVerbose(0);
    imap.setVerbose(0);
    mltk::ensemble::VotingClassifier<double> voting(wine, "hard", bp, knn, imap);
    ASSERT_GT(mltk::validation::kfold(wine, voting, 10, 0).accuracy, 60);
}