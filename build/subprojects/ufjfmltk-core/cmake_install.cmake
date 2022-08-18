# Install script for directory: /home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/build/lib/libcore.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ufjfmltk/core" TYPE FILE FILES
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Data.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Datasets.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Point.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/DistanceMetric.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Kernel.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Learner.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Point.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Random.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Sampling.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Solution.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Statistics.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Timer.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/Utils.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/ExprOps.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/ExprScalar.hpp"
    "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/core/ExprTraits.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ufjfmltk" TYPE FILE FILES "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/subprojects/ufjfmltk-core/include/ufjfmltk/Core.hpp")
endif()

