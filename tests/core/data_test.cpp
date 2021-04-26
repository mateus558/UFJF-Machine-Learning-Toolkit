//
// Created by mateus on 12/04/2021.
//

#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"

TEST(DataTest, OpenDataBinDataset) {
    mltk::Data<double> data("iris.data");
    std::vector<size_t> dist = {50, 100};

    EXPECT_EQ(data.size(), 150);
    EXPECT_EQ(data.points().size(), 150);
    EXPECT_EQ(data.dim(), 4);
    EXPECT_EQ(data.classesDistribution(), dist);
}

TEST(DataTest, OpenCsvMultiDataset) {
    mltk::Data<double> data("iris_mult.csv");
    std::vector<size_t> dist = {50, 50, 50};

    EXPECT_EQ(data.size(), 150);
    EXPECT_EQ(data.points().size(), 150);
    EXPECT_EQ(data.classes().size(), 3);
    EXPECT_EQ(data.dim(), 4);
    EXPECT_EQ(data.classesDistribution(), dist);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}