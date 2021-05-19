//
// Created by mateus on 12/04/2021.
//

#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/valid/Validation.hpp"

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

    mltk::Data<double> seismic("seismic-bumps.arff", true);
    auto seis_dist = seismic.classesDistribution();
    EXPECT_EQ(seismic.size(), 2584);
    EXPECT_EQ(seismic.points().size(), 2584);
    EXPECT_EQ(seismic.dim(), 18);
    EXPECT_EQ(std::accumulate(seis_dist.begin(), seis_dist.end(),0), seismic.size());

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

    mltk::Data<double> txt("teste.txt");
    EXPECT_EQ(txt.size(), 4);
    EXPECT_EQ(txt.points().size(), 4);
    EXPECT_EQ(txt.classes().size(), 0);
    EXPECT_EQ(txt.dim(), 3);
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
    mltk::Data<double> cp = bin, cp_zero;

    EXPECT_EQ(cp.size(), 150);
    EXPECT_EQ(cp.points().size(), 150);
    EXPECT_EQ(cp.dim(), 4);
    EXPECT_EQ(cp.classesDistribution(), dist);
    EXPECT_EQ(cp.classes().size(), 2);

    cp_zero.copyZero(bin);
    EXPECT_EQ(cp_zero.size(), 0);

    EXPECT_EQ(bin.size(), 150);
    EXPECT_EQ(bin.points().size(), 150);
    EXPECT_EQ(bin.dim(), 4);
    EXPECT_EQ(bin.classesDistribution(), dist);
    EXPECT_EQ(bin.classes().size(), 2);
}

TEST_F(DataTest, CopyMultDataset) {
    std::vector<size_t> dist = {50, 50, 50};
    mltk::Data<double> cp = mult, copy_mult;

    copy_mult = mult.copy();
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

    mltk::Data<double> floatData(5, 5);
    for(auto& p: floatData){
        mltk::random_init(*p, 42);
        *p *= 10;
        *p = mltk::normalize(*p, 2);
        std::cout << *p << std::endl;
        ASSERT_EQ(p->norm(), 1);
    }
    std::cout << floatData << std::endl;
    ASSERT_EQ(floatData.size(), 5);
    ASSERT_EQ(floatData.dim(), 5);
    auto new_point = mltk::random_init<double>(floatData.dim(), 42);
    floatData.insertPoint(new_point);
    ASSERT_EQ(floatData.size(), 6);
    ASSERT_EQ(floatData.dim(), 5);
    ASSERT_EQ(floatData.classes().size(), 1);
    auto new_point1 = mltk::random_init<double>(floatData.dim(), 42);
    new_point1.Y() = 1;
    std::vector<size_t> dist1 = {floatData.size(),1};
    floatData.insertPoint(new_point1);
    std::vector<size_t> dist2 = {floatData.size()};
    ASSERT_EQ(floatData.size(), 7);
    ASSERT_EQ(floatData.dim(), 5);
    ASSERT_EQ(floatData.classes().size(), 2);
    floatData.computeClassesDistribution();
    ASSERT_EQ(floatData.classesDistribution(), dist1);
    floatData.updatePointValue(floatData.size()-1, 0);
    ASSERT_EQ(floatData.classesDistribution(), dist2);
    ASSERT_EQ(floatData.classes().size(), 1);
    ASSERT_EQ(floatData.getLabels().size(), floatData.size());

    ASSERT_EQ(mltk::utils::atod("0.151"), 0.151);
    ASSERT_EQ(mltk::utils::atod("-0.151"), -0.151);
    ASSERT_GT(mltk::utils::atod(mltk::utils::dtoa(0.151).c_str()), 0.150);
    ASSERT_LT(mltk::utils::atod(mltk::utils::dtoa(0.151).c_str()), 0.152);
    ASSERT_GT(mltk::utils::atod(mltk::utils::dtoa(-0.151).c_str()), -0.152);
    ASSERT_LT(mltk::utils::atod(mltk::utils::dtoa(-0.151).c_str()), -0.150);
    ASSERT_STREQ(mltk::utils::dtoa(1e-3).c_str(), "0.001");
    ASSERT_STREQ(mltk::utils::itos(1000).c_str(), "1000");
    ASSERT_STREQ(mltk::utils::itos(-1000).c_str(), "-1000");
    ASSERT_STREQ(mltk::utils::dtoa(1E14).c_str(), "1e+14");
    ASSERT_STREQ(mltk::utils::dtoa(1E14).c_str(), "1e+14");
    ASSERT_STREQ(mltk::utils::dtoa(3E-14).c_str(), "3e-14");
}

TEST_F(DataTest, DataSplit){
    mltk::Data<double> data("pima.data");
    auto split = mltk::validation::kfoldsplit(mult, 10, true, 42);
    std::vector<size_t> dist_train = {45, 45, 45};
    std::vector<size_t> dist_test = {5, 5, 5};
    for(const auto& s: split){
        ASSERT_EQ(s.train.size(), 135);
        ASSERT_EQ(s.test.size(), 15);
        ASSERT_EQ(s.train.classesDistribution(), dist_train);
        ASSERT_EQ(s.test.classesDistribution(), dist_test);
    }

    split = mltk::validation::kfoldsplit(mult, 10, 10, true, 42);
    ASSERT_EQ(split.size(), 100);
    for(const auto& s: split){
        ASSERT_EQ(s.train.size(), 135);
        ASSERT_EQ(s.test.size(), 15);
        ASSERT_EQ(s.train.classesDistribution(), dist_train);
        ASSERT_EQ(s.test.classesDistribution(), dist_test);
    }
    split = mltk::validation::kfoldsplit(mult, 3, false, 42);
    for(const auto& s: split){
        ASSERT_EQ(s.train.size(), 100);
        ASSERT_EQ(s.test.size(), 50);
        std::cout << mltk::Point<size_t>(s.train.classesDistribution()) <<std::endl;
        std::cout << mltk::Point<size_t>(s.test.classesDistribution()) <<std::endl;
    }
}

TEST_F(DataTest, DataTransformation){
    auto normalization = [](mltk::PointPointer<double> point){
        *point = mltk::normalize(*point, 2);
    };
    bin.apply(normalization);
    for(const auto p: bin){
        EXPECT_GT(p->norm(), 0.999);
        EXPECT_LT(p->norm(), 1.01);
    }
}