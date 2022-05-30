//
// Created by mateus on 29/04/2021.
//

#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"

class StatsTest: public ::testing::Test
{
protected:
    void SetUp() override{
        if(!data.load("iris_mult.csv")){
            std::cerr << "Error loading multiclass dataset." << std::endl;
        }
    }
    mltk::Data<double> data;
};

TEST_F(StatsTest, CentralTendencyMeasures){
    ASSERT_GT(mltk::stats::mean(data, 0), 5.8);
    ASSERT_LT(mltk::stats::mean(data, 0), 6);
    ASSERT_GT(mltk::stats::radius(data, -1,2), 3.83);
    ASSERT_LT(mltk::stats::radius(data, -1,2), 3.84);
    ASSERT_EQ(mltk::stats::radius(data, -1,1), 7.9);
    ASSERT_GT(mltk::stats::distCenters(data, -1), 3.97);
    ASSERT_LT(mltk::stats::distCenters(data, -1), 3.973);
}

TEST_F(StatsTest, SpreadMeasures){
    ASSERT_GT(mltk::stats::var(data, -1), 4.53);
    ASSERT_LT(mltk::stats::var(data, -1), 4.54);
    ASSERT_GT(mltk::stats::var(*data[1]), 3.11);
    ASSERT_LT(mltk::stats::var(*data[1]), 3.12);
    ASSERT_GT(mltk::stats::std_dev(data, 1), 0.43);
    ASSERT_LT(mltk::stats::std_dev(data, 1), 0.44);
    ASSERT_GT(mltk::stats::std_dev(*data[1]), 1.76);
    ASSERT_LT(mltk::stats::std_dev(*data[1]), 1.77);
    ASSERT_GT(mltk::stats::covar(data.getFeature(1), data.getFeature(0)), -0.04);
    ASSERT_LT(mltk::stats::covar(data.getFeature(1), data.getFeature(0)), -0.03);
}
