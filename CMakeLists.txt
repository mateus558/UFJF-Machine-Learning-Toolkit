cmake_minimum_required(VERSION 3.16)
project(UFJF-mltk VERSION 1.0.0)


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
add_subdirectory(Exec)
add_subdirectory(GUI)