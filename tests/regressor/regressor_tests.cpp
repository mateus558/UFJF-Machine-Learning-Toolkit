//
// Created by mateus on 25/05/2021.
//

#include "ufjfmltk/Core.hpp"
#include "ufjfmltk/Regressor.hpp"
#include "ufjfmltk/Visualize.hpp"
#include <gtest/gtest.h>

class RegressorTests: public ::testing::Test
{
protected:
    void SetUp() override{
        reg2d = mltk::datasets::make_regression(100, 2, 0, 2,  0, 30).dataset;
        reg1d = mltk::datasets::make_regression(100, 1, 2, 0.01).dataset;
    }
    mltk::Data<double> reg1d, reg2d;
};

TEST_F(RegressorTests, Tests1D){
    std::cout << reg1d.getType() << std::endl;
    std::cout << reg1d.name() << std::endl;
    std::cout << "dim: " << reg1d.dim() << std::endl;

    mltk::visualize::Visualization<> vis(reg1d, false);
    vis.setTerminal("dumb");
    vis.plot1DRegresion();

    mltk::regressor::LMSPrimal<double> lms(reg1d, 0.5, 3);

    ASSERT_TRUE(lms.train());
    mltk::Point<double> predictions(reg1d.size());

    for(size_t i = 0; i < reg1d.size(); i++){
        predictions[i] = lms.evaluate(reg1d(i));
    }
    ASSERT_LT(mltk::metrics::MSE(reg1d.getLabels(), predictions), 10);
    auto s = lms.getSolution();
    vis.plot1DRegresionHyperplane(0, s);
}

TEST_F(RegressorTests, Tests2D){
    std::cout << reg2d.getType() << std::endl;
    std::cout << reg2d.name() << std::endl;
    std::cout << "dim: " << reg2d.dim() << std::endl;

    mltk::visualize::Visualization<> vis(reg2d, false);
    vis.setTerminal("dumb");
    vis.plot2DRegresion();

    mltk::regressor::LMSPrimal<double> lms(reg2d, 0.001, 3);

    ASSERT_TRUE(lms.train());
    mltk::Point<double> predictions(reg2d.size());

    for(size_t i = 0; i < reg2d.size(); i++){
        predictions[i] = lms.evaluate(reg2d(i));
    }
    ASSERT_LT(mltk::metrics::MSE(reg2d.getLabels(), predictions), 10);
    auto s = lms.getSolution();
    vis.plot2DRegresionHyperplane(0, 1, s);
}