//
// Created by mateus on 25/05/2021.
//

#include "ufjfmltk/Core.hpp"
#include "ufjfmltk/Regressor.hpp"
#include "ufjfmltk/Visualize.hpp"
#include <gtest/gtest.h>

TEST(RegresorTests, Tests1D){
    auto regressor_data = mltk::datasets::make_regression(100, 1, 2, 0.01).dataset;

    std::cout << regressor_data.getType() << std::endl;
    std::cout << regressor_data.name() << std::endl;
    std::cout << "dim: " << regressor_data.dim() << std::endl;

    mltk::visualize::Visualization<> vis(regressor_data, false);
    vis.setTerminal("dummy");
    vis.plot1DRegresion();

    mltk::regressor::LMSPrimal<double> lms(regressor_data, 0.5, 3);

    lms.train();

    auto s = lms.getSolution();
    vis.plot1DRegresionHyperplane(0, s);
}

TEST(RegresorTests, Tests2D){
    auto regressor_data = mltk::datasets::make_regression(100, 2, 0, 2).dataset;

    std::cout << regressor_data.getType() << std::endl;
    std::cout << regressor_data.name() << std::endl;
    std::cout << "dim: " << regressor_data.dim() << std::endl;

    mltk::visualize::Visualization<> vis(regressor_data, false);
    vis.setTerminal("dummy");
    vis.plot2DRegresion();

    mltk::regressor::LMSPrimal<double> lms(regressor_data, 0.001, 3);

    lms.train();

    auto s = lms.getSolution();
    vis.plot2DRegresionHyperplane(0, 1, s);
}