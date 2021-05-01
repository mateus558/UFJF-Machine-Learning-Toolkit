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
    dist = {540, 611};
    mltk::Data<double> arff("diabetic.arff", true);
    EXPECT_EQ(arff.size(), 1151);
    EXPECT_EQ(arff.points().size(), 1151);
    EXPECT_EQ(arff.classes().size(), 2);
    EXPECT_EQ(arff.dim(), 19);
    EXPECT_EQ(arff.classesDistribution(), dist);
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
    mltk::Data<double> cp = mult, copy_mult;

    copy_mult.copy(mult);
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

    mult.shuffle();
    ASSERT_TRUE(mult != copy_mult);
    ASSERT_FALSE(mult == copy_mult);
    mult.normalize();
}

TEST_F(DataTest, PointRemoval) {
    std::set<int> ids_remove;
    std::vector<int> ids_remove_v;
    int n = bin.size() * 0.5;
    std::vector<size_t> dist = {50, 100};
    mltk::random::init(10);
    mltk::Data<double> bin_copy;
    bin_copy.copy(bin);
    EXPECT_EQ(bin_copy.size(), 150);
    EXPECT_EQ(bin_copy.points().size(), 150);
    EXPECT_EQ(bin_copy.dim(), 4);
    EXPECT_EQ(bin_copy.classesDistribution(), dist);
    EXPECT_EQ(bin_copy.classes().size(), 2);

    for (int i = 0; i < n; i++) {
        ids_remove.insert(mltk::random::intInRange(1, n));
    }

    if (std::find(ids_remove.begin(), ids_remove.end(), 1) == ids_remove.end()) {
        ids_remove.insert(1);
    }
    if (std::find(ids_remove.begin(), ids_remove.end(), bin.size() - 1) == ids_remove.end()) {
        ids_remove.insert(bin.size());
    }

    for (auto idx: ids_remove) {
        ids_remove_v.push_back(idx);
        if (!bin.removePoint(idx)) {
            std::clog << "Point " << idx << " not removed." << std::endl;
        }
    }
    auto sum_dist = mltk::Point<size_t>(bin.classesDistribution()).sum();
    EXPECT_EQ(bin.size(), 150 - ids_remove.size());
    EXPECT_EQ(bin.points().size(), 150 - ids_remove.size());
    EXPECT_EQ(bin.dim(), 4);
    EXPECT_EQ(bin.size(), sum_dist);
    EXPECT_EQ(bin.classes().size(), 2);
    EXPECT_EQ(bin_copy.size(), 150);
    EXPECT_EQ(bin_copy.points().size(), 150);
    EXPECT_EQ(bin_copy.dim(), 4);
    EXPECT_EQ(bin_copy.classesDistribution(), dist);
    EXPECT_EQ(bin_copy.classes().size(), 2);
    EXPECT_NE(bin, bin_copy);

    bin_copy.removePoints(ids_remove_v);
    sum_dist = mltk::Point<size_t>(bin_copy.classesDistribution()).sum();
    EXPECT_EQ(bin_copy.size(), 150 - ids_remove_v.size());
    EXPECT_EQ(bin_copy.points().size(), 150 - ids_remove_v.size());
    EXPECT_EQ(bin_copy.dim(), 4);
    EXPECT_EQ(bin_copy.size(), sum_dist);
    EXPECT_EQ(bin_copy.classes().size(), 2);

    EXPECT_TRUE(bin == bin_copy);
    bin_copy.removeFeatures({1, 2});
    ASSERT_TRUE(bin_copy.dim() == 2);
}

TEST_F(DataTest, DataInsertion){
    mltk::Data<double> copy_bin;
    std::vector<size_t> dist = {50, 100, 1};
    std::vector<int> classes = {1, -1, 2};
    copy_bin.copy(bin);

    copy_bin.insertPoint(mult, 70);
    ASSERT_EQ(copy_bin.size(), bin.size()+1);
    ASSERT_EQ(copy_bin.classesDistribution(), dist);
    ASSERT_EQ(copy_bin.classes(), classes);

    auto red = copy_bin.insertFeatures({1,2});
    auto red1 = copy_bin.selectFeatures({1,2});

    for(int i = 0; i < copy_bin.size(); i++){
        ASSERT_EQ((*copy_bin[i])[1], (*red1[i])[0]);
        ASSERT_EQ((*copy_bin[i])[2], (*red1[i])[1]);
    }

    for(int i = 0; i < copy_bin.size(); i++){
        ASSERT_EQ((*copy_bin[i])[0], (*((*red)[i]))[0]);
        ASSERT_EQ((*copy_bin[i])[1], (*((*red)[i]))[1]);
    }

    auto sample = mult.sampling(75);
    std::vector<size_t> samp_dist = {25,25,25};
    ASSERT_EQ(sample.classesDistribution(), samp_dist);
    std::cout << mltk::normalize(*sample[0], 2) << std::endl;
    mltk::Data<double> floatData(5, 5);
    for(auto& p: floatData){
        mltk::random_init(*p, 42);
    }
    ASSERT_EQ(floatData.size(), 5);
    ASSERT_EQ(floatData.dim(), 5);
    ASSERT_EQ(mltk::utils::atod("0.151"), 0.151);
    ASSERT_EQ(mltk::utils::atod("-0.151"), -0.151);
    ASSERT_GT(mltk::utils::atod(mltk::utils::dtoa(0.151).c_str()), 0.150);
    ASSERT_LT(mltk::utils::atod(mltk::utils::dtoa(0.151).c_str()), 0.152);
    ASSERT_GT(mltk::utils::atod(mltk::utils::dtoa(-0.151).c_str()), -0.152);
    ASSERT_LT(mltk::utils::atod(mltk::utils::dtoa(-0.151).c_str()), -0.150);
}