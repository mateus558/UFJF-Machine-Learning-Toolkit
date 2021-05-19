//
// Created by mateus on 18/05/2021.
//
#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/Visualize.hpp"
#include "ufjfmltk/Classifier.hpp"


class VisualTest: public ::testing::Test
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

TEST_F(VisualTest, PlotsTest){
    mltk::visualize::Visualization<double> vis;
    vis.setSample(bin);
    vis.setTerminal("dumb");
    vis.plot2D(0, 1, true, "Iris dataset 2D");
    vis.plot2D(0, 2, true, "Iris dataset 2D");
    vis.plot2D(0, 3, true, "Iris dataset 2D");
    vis.plot2D(2, 0, true, "Iris dataset 2D");
    vis.plot2D(2, 1, true, "Iris dataset 2D");
    vis.plot2D(2, 3, true, "Iris dataset 2D");
    vis.plot2D(3, 0, false, "Iris dataset 2D");
    vis.plot2D(3, 1, false, "Iris dataset 2D");
    vis.plot2D(3, 2, false, "Iris dataset 2D");
    vis.plot2D(1, 0, false, "Iris dataset 2D");
    vis.plot2D(1, 2, false, "Iris dataset 2D");
    vis.plot2D(1, 3, false, "Iris dataset 2D");
    vis.plot3D(1, 2, 3, true, "Iris dataset 3D");
    vis.plot3D(1, 3, 2, true, "Iris dataset 3D");
    vis.plot3D(3, 1, 2, true, "Iris dataset 3D");
    vis.plot3D(3, 2, 1, false, "Iris dataset 3D");
    vis.plot3D(2, 3, 1, false, "Iris dataset 3D");
    vis.plot3D(2, 1, 3, false, "Iris dataset 3D");
    vis.plot3D(0, 1, 3, false, "Iris dataset 3D");
}

TEST_F(VisualTest, PlotWithSolutionTest){
    mltk::classifier::IMAp<double> ima(bin);
    mltk::visualize::Visualization<double> vis(bin, false);

    vis.setTerminal("dumb");
    ima.setVerbose(0);
    ima.train();

    auto solution = ima.getSolution();
    vis.plot2DwithHyperplane(0, 1, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(0, 2, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(0, 3, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 0, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 1, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 3, solution, true, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 0, solution, false, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 1, solution, false, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 2, solution, false, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 0, solution, false, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 2, solution, false, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 3, solution, false, "Iris dataset 2D");

    vis.setTitle("2D test");
    vis.setStyle("qualquer");
    vis.plot3DwithHyperplane(0, 1, 2, solution, true, "Iris dataset 3D");
    vis.plot3DwithHyperplane(1, 2, 3, solution, false, "Iris dataset 3D");
}