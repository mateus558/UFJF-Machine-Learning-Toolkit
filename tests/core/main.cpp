//
// Created by mateus on 29/04/2021.
//
#include <gtest/gtest.h>
#ifdef CODECOV_ENABLED
extern "C" void __gcov_flush();
#include <gcov.h>
#endif

int main(int argc, char *argv[]) {
#ifdef CODECOV_ENABLED
    __gcov_flush();
#endif
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}