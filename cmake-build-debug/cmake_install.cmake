# Install script for directory: /home/mateuscmarim/Repositories/UFJF-MLTK

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/mateuscmarim/Repositories/UFJF-MLTK/cmake-build-debug/libUFJFmltk.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUFJFmltk.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/UFJF-MLTK" TYPE FILE FILES
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Data.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Statistics.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Utils.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Point.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Classifier.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Clusterer.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/DualClassifier.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/DualRegressor.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Ensemble.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/FeatureSelection.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Fisher.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/gnuplot_i.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Golub.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/IMA.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Kernel.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/KMeans.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/KNN.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Learner.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/LMS.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/MLToolkit.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/OneVsAll.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Perceptron.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Point.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/PrimalClassifier.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/PrimalCommittee.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/PrimalPercCommittee.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/PrimalRegressor.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Random.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Regressor.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/RFE.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/SMO.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Solution.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Timer.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Validation.hpp"
    "/home/mateuscmarim/Repositories/UFJF-MLTK/includes/Visualization.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK")
      file(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK"
           RPATH "")
    endif()
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/mateuscmarim/Repositories/UFJF-MLTK/cmake-build-debug/UFJF-MLTK")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK")
      file(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK"
           OLD_RPATH "/home/mateuscmarim/Repositories/UFJF-MLTK/cmake-build-debug:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/UFJF-MLTK")
      endif()
    endif()
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/mateuscmarim/Repositories/UFJF-MLTK/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
