cmake_minimum_required(VERSION 3.16)
project(UFJF-mltk VERSION 1.0.0)

option(BUILD_GUI "Tells if the Qt GUI application must be built." OFF)
option(BUILD_CLI "Tells if the console application must be built." ON)
option(BUILD_LIBVISUALIZE "Tells if the visualization module must be built." ON)
option(BUILD_LIBCLASSIFIER "Tells if the classifier module must be built." ON)
option(BUILD_LIBREGRESSOR "Tells if the regressor module must be built." ON)
option(BUILD_LIBCLUSTERER "Tells if the clusterer module must be built." ON)
option(BUILD_LIBFEATSELECT "Tells if the feature selection module must be built." ON)
option(BUILD_LIBVALIDATION "Tells if the validation module must be built." ON)

set(LIBNAME libUFJFmltk)
set(LIBCORE core)
set(LIBVISUALIZE visualize)
set(LIBCLASSIFIER classifier)
set(LIBREGRESSOR regressor)
set(LIBCLUSTERER clusterer)
set(LIBFEATSELECT feature-selection)
set(LIBVALIDATION validation)

message(STATUS "UFJF-MLTK Version: ${PROJECT_VERSION}")

add_subdirectory(Modules)

if(BUILD_CLI)
    add_subdirectory(CLI)
endif()

if(BUILD_GUI)
    add_subdirectory(GUI)
endif()