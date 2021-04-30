//
// Created by mateus on 12/04/2021.
//

#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"

class DataTest: public ::testing::Test
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


TEST_F(DataTest, OpenDataBinDataset) {
    std::vector<size_t> dist = {50, 100};

    EXPECT_EQ(bin.size(), 150);
    EXPECT_EQ(bin.points().size(), 150);
    EXPECT_EQ(bin.dim(), 4);
    EXPECT_EQ(bin.classesDistribution(), dist);

    bin.write("_iris", "data");
    bin.write("_iris", "csv");
    bin.write("_iris", "plt");

    mltk::Data<double> data("_iris.data");
    EXPECT_EQ(data.size(), 150);
    EXPECT_EQ(data.points().size(), 150);
    EXPECT_EQ(data.dim(), 4);
    EXPECT_EQ(data.classesDistribution(), dist);

    mltk::Data<double> csv("_iris.csv");
    EXPECT_EQ(csv.size(), 150);
    EXPECT_EQ(csv.points().size(), 150);
    EXPECT_EQ(csv.dim(), 4);
    EXPECT_EQ(csv.classesDistribution(), dist);
}

TEST_F(DataTest, OpenCsvMultiDataset) {
    std::vector<size_t> dist = {50, 50, 50};

    EXPECT_EQ(mult.size(), 150);
    EXPECT_EQ(mult.points().size(), 150);
    EXPECT_EQ(mult.classes().size(), 3);
    EXPECT_EQ(mult.dim(), 4);
    EXPECT_EQ(mult.classesDistribution(), dist);

    mult.write("_iris", "data");
    mult.write("_iris", "csv");
    mult.write("_iris", "plt");

    mltk::Data<double> data("_iris.data");
    EXPECT_EQ(data.size(), 150);
    EXPECT_EQ(data.points().size(), 150);
    EXPECT_EQ(data.classes().size(), 3);
    EXPECT_EQ(data.dim(), 4);
    EXPECT_EQ(data.classesDistribution(), dist);

    mltk::Data<double> csv("_iris.csv");
    EXPECT_EQ(csv.size(), 150);
    EXPECT_EQ(csv.points().size(), 150);
    EXPECT_EQ(csv.classes().size(), 3);
    EXPECT_EQ(csv.dim(), 4);
    EXPECT_EQ(csv.classesDistribution(), dist);
}

TEST_F(DataTest, CopyBinDataset) {
    std::vector<size_t> dist = {50, 100};
    mltk::Data<double> cp = bin;

    EXPECT_EQ(cp.size(), 150);
    EXPECT_EQ(cp.points().size(), 150);
    EXPECT_EQ(cp.dim(), 4);
    EXPECT_EQ(cp.classesDistribution(), dist);
    EXPECT_EQ(cp.classes().size(), 2);

    EXPECT_EQ(bin.size(), 150);
    EXPECT_EQ(bin.points().size(), 150);
    EXPECT_EQ(bin.dim(), 4);
    EXPECT_EQ(bin.classesDistribution(), dist);
    EXPECT_EQ(bin.classes().size(), 2);
}

TEST_F(DataTest, CopyMultDataset) {
    std::vector<size_t> dist = {50, 50, 50};
    mltk::Data<double> cp = mult;

    EXPECT_EQ(cp.size(), 150);
    EXPECT_EQ(cp.points().size(), 150);
    EXPECT_EQ(cp.dim(), 4);
    EXPECT_EQ(cp.classesDistribution(), dist);
    EXPECT_EQ(cp.classes().size(), 3);

    EXPECT_EQ(mult.size(), 150);
    EXPECT_EQ(mult.points().size(), 150);
    EXPECT_EQ(mult.dim(), 4);
    EXPECT_EQ(mult.classesDistribution(), dist);
    EXPECT_EQ(mult.classes().size(), 3);
}