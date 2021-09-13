//
// Created by mateus on 13/09/2021.
//

#include <iostream>

#include <gtest/gtest.h>
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/core/Datasets.hpp"
#include "ufjfmltk/core/Kernel.hpp"
#include "ufjfmltk/core/Sampling.hpp"
#include "ufjfmltk/valid/Validation.hpp"
#include <ufjfmltk/visual/Visualization.hpp>

class SamplingTest: public ::testing::Test
        {
        protected:
            void SetUp() override{
                if(!bin.load("iris.data")){
                    std::cerr << "Error loading binary dataset." << std::endl;
                }
            }
            mltk::Data<double> bin;
        };


TEST_F(SamplingTest, SMOTETest) {
    mltk::SMOTE smote;
    auto smote_bin = smote(bin);

    ASSERT_EQ(smote_bin.size(), 200);
}