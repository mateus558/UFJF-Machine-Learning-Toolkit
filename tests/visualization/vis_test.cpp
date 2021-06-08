//
// Created by mateus on 18/05/2021.
//
#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/core/Datasets.hpp"
#include "ufjfmltk/Visualize.hpp"
#include "ufjfmltk/Classifier.hpp"
#include "ufjfmltk/Validation.hpp"

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
    vis.plot2D(0, 1, true, 1, "Iris dataset 2D");
    vis.plot2D(0, 2, true, 1, "Iris dataset 2D");
    vis.plot2D(0, 3, true, 1, "Iris dataset 2D");
    vis.plot2D(2, 0, true, 1, "Iris dataset 2D");
    vis.plot2D(2, 1, true, 1, "Iris dataset 2D");
    vis.plot2D(2, 3, true, 1, "Iris dataset 2D");
    vis.plot2D(3, 0, false, 1, "Iris dataset 2D");
    vis.plot2D(3, 1, false, 1, "Iris dataset 2D");
    vis.plot2D(3, 2, false, 1, "Iris dataset 2D");
    vis.plot2D(1, 0, false, 1, "Iris dataset 2D");
    vis.plot2D(1, 2, false, 1, "Iris dataset 2D");
    vis.plot2D(1, 3, false, 1, "Iris dataset 2D");
    vis.plot3D(1, 2, 3, true, 1, "Iris dataset 3D");
    vis.plot3D(1, 3, 2, true, 1, "Iris dataset 3D");
    vis.plot3D(3, 1, 2, true, 1, "Iris dataset 3D");
    vis.plot3D(3, 2, 1, false, 1, "Iris dataset 3D");
    vis.plot3D(2, 3, 1, false, 1, "Iris dataset 3D");
    vis.plot3D(2, 1, 3, false, 1, "Iris dataset 3D");
    vis.plot3D(0, 1, 3, false, 1, "Iris dataset 3D");
}

TEST_F(VisualTest, PlotWithSolutionTest){
    mltk::classifier::IMAp<double> ima(bin);
    mltk::visualize::Visualization<double> vis(bin, false);

    vis.setTerminal("dumb");
    ima.setVerbose(1);
    ima.setMaxTime(2000);
    ima.train();

    auto solution = ima.getSolution();
    vis.plot2DwithHyperplane(0, 1, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(0, 2, solution, true, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(0, 3, solution, true, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 0, solution, true, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 1, solution, true, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(2, 3, solution, true, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 0, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 1, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(3, 2, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 0, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 2, solution, false, 1, "Iris dataset 2D");
    vis.plot2DwithHyperplane(1, 3, solution, false, 1, "Iris dataset 2D");

    vis.setTitle("2D test");
    vis.setStyle("qualquer");

    vis.plot3DwithHyperplane(0, 1, 2, solution, true, 1, "Iris dataset 3D");
    vis.plot3DwithHyperplane(1, 2, 3, solution, true, 1, "Iris dataset 3D");
}

TEST_F(VisualTest, ContourTest){
    mltk::classifier::KNNClassifier<double, mltk::metrics::dist::Hassanat<double>> knn(mult, 3);
    mltk::classifier::KNNClassifier knn_euc(mult, 3);
    mltk::classifier::IMADual ima(bin, mltk::KernelType::GAUSSIAN, 0.5);

    mltk::visualize::Visualization<double> vis(mult, false);
    vis.setTerminal("dumb");
    auto script = vis.plotDecisionSurface2D(knn, 0, 1, false, 100, false, 1.3);
    std::ofstream fscript("decision.script");
    fscript << script;
    fscript.close();
    mltk::visualize::Visualization<double> vis2(mult, false);
    vis2.setTerminal("dumb");
    vis2.plotDecisionSurface2D(knn_euc, 0, 1, false, 100, true, 1, "KNN euclidean");
    mltk::visualize::Visualization<double> vis1(bin, false);
    vis1.setTerminal("dumb");
    vis1.plotDecisionSurface2D(ima, 0, 1, false, 100, true, 1, "IMA dual decision surface");
}

TEST_F(VisualTest, PlotArtificial){
    auto spirals = mltk::datasets::make_spirals(800, 2, false, 2.5);
    auto blobs = mltk::datasets::make_blobs(100, 2, 2).dataset;
    mltk::classifier::IMADual<> ima(spirals, mltk::GAUSSIAN, 100);
    mltk::visualize::Visualization<> vis(spirals, true, true);

    std::cout << blobs.name() << " dataset\n";
    std::cout << "dims: " << blobs.dim() << std::endl;
    std::cout << "size: " << blobs.size() << std::endl;
    std::cout << "classes: " << mltk::Point<int>(blobs.classes()) << std::endl;
    std::cout << "classes distribution: " << mltk::Point<size_t>(blobs.classesDistribution()) << std::endl;
    std::cout << std::endl;
    std::cout << spirals.name() << " dataset\n";
    std::cout << "dims: " << spirals.dim() << std::endl;
    std::cout << "size: " << spirals.size() << std::endl;
    std::cout << "classes: " << mltk::Point<int>(spirals.classes()) << std::endl;
    std::cout << "classes distribution: " << mltk::Point<size_t>(spirals.classesDistribution()) << std::endl;

    vis.setTerminal("dumb");
    vis.plot2D(0,1);
    std::cout << vis.plotDecisionSurface2D(ima, 0, 1, false, 50,  false, 1.2) << std::endl;
    vis.setSample(blobs);
    vis.plot2D(0,1);
    std::cout << vis.plotDecisionSurface2D(ima, 0, 1, false, 50,  false, 1.2) << std::endl;
    vis.plot3D(0,1,0);
    spirals.normalize();
    vis.setSample(spirals);
    std::cout << vis.plot3D(0,1,2) << std::endl;
    vis.plot2D(1,2);
}