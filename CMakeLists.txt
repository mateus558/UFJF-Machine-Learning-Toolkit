cmake_minimum_required(VERSION 2.8)

project(UFJF-MLTK)
file(GLOB SOURCES
        includes/*.hpp
        src/*.cpp
        )

include_directories(includes)
include_directories(src)

add_library(UFJFmltk SHARED
        src/AOS.cpp
        src/Data.cpp
        src/Fisher.cpp
        src/Golub.cpp
        src/IMA.cpp
        src/KMeans.cpp
        src/KNN.cpp
        src/Kernel.cpp
        src/LMS.cpp
        src/OneVsAll.cpp
        src/Perceptron.cpp
        src/PrimalCommittee.cpp
        src/PrimalPercCommittee.cpp
        src/RFE.cpp
        src/SMO.cpp
        src/Statistics.cpp
        src/Utils.cpp
        src/Validation.cpp
        src/Visualization.cpp
        src/gnuplot_i.cpp
        )

set_target_properties(UFJFmltk PROPERTIES PUBLIC_HEADER "includes/Data.hpp;includes/Statistics.hpp;includes/Utils.hpp;includes/Point.hpp;includes/Classifier.hpp;includes/Clusterer.hpp;includes/DualClassifier.hpp;includes/DualRegressor.hpp;includes/Ensemble.hpp;includes/FeatureSelection.hpp;includes/Fisher.hpp;includes/gnuplot_i.hpp;includes/Golub.hpp;includes/IMA.hpp;includes/Kernel.hpp;includes/KMeans.hpp;includes/KNN.hpp;includes/Learner.hpp;includes/LMS.hpp;includes/MLToolkit.hpp;includes/OneVsAll.hpp;includes/Perceptron.hpp;includes/Point.hpp;includes/PrimalClassifier.hpp;includes/PrimalCommittee.hpp;includes/PrimalPercCommittee.hpp;includes/PrimalRegressor.hpp;includes/Random.hpp;includes/Regressor.hpp;includes/RFE.hpp;includes/SMO.hpp;includes/Solution.hpp;includes/Timer.hpp;includes/Validation.hpp;includes/Visualization.hpp")


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} UFJFmltk)


include(GNUInstallDirs)

install(TARGETS UFJFmltk
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/UFJF-MLTK
        )

install(TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug)