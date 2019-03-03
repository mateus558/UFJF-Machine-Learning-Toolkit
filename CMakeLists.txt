cmake_minimum_required(VERSION 2.8)

project(UFJF-MLTK)
file(GLOB SOURCES
    includes/*.hpp
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} includes/AOS.hpp src/AOS.cpp includes/Learner.hpp includes/Regressor.hpp)