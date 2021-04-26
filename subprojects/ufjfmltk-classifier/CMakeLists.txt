# This is the "physical" (i.e. real) name of the library.
# The actual file name of the library built is constructed based on conventions
# of the native platform, such as lib<LIBRARY_TARGET_NAME>.a,
# lib<LIBRARY_TARGET_NAME>.dylib or <LIBRARY_TARGET_NAME>.lib.
#
# In order to use a library name LIBRARY_TARGET_NAME that is not the same as the
# `EXPORT` used in the `install` command (see the end of this file), it is
# required to use `FIRST_TARGET` option of the `install_basic_package_files`
# command in the main CMakeLists.txt file with the name of one of the exported
# targets. `FIRST_TARGET` option defaults to LIBRARY_TARGET_NAME.
# By using setting LIBRARY_TARGET_NAME to ${PROJECT_NAME} it is not necessary to
# use `FIRST_TARGET` option in the main CMakeLists.txt.
# In general, in a C++ project, there is at least one exported target whose
# name is always know. Consequently, `FIRST_TARGET` option in the main
# CMakeLists.txt can be statically set to such target name. In the context of
# projects where users may choose what target to export, thus the name of a
# target may change at configure time, you can use propagate back to parent
# CMakeLists.txt, up to the main one, a variable containing the name of a
# target. In order to do it, you can use `set` command with `PARENT_SCOPE`
# option, see https://cmake.org/cmake/help/latest/command/set.html.
set(CLASSIFIER_TARGET ${PROJECT_NAME}-classifier)

# List of CPP (source) library files.
set(${CLASSIFIER_TARGET}_SRC
        src/IMA.cpp
        src/Perceptron.cpp
        src/SMO.cpp
        )

# List of HPP (header) library files.
set(${CLASSIFIER_TARGET}_HDR
        include/ufjfmltk/classifier/Classifier.hpp
        include/ufjfmltk/classifier/DualClassifier.hpp
        include/ufjfmltk/classifier/IMA.hpp
        include/ufjfmltk/classifier/KNNClassifier.hpp
        include/ufjfmltk/classifier/OneVsAll.hpp
        include/ufjfmltk/classifier/OneVsOne.hpp
        include/ufjfmltk/classifier/Perceptron.hpp
        include/ufjfmltk/classifier/PrimalClassifier.hpp
        include/ufjfmltk/classifier/SMO.hpp
        include/ufjfmltk/classifier/hnswlib/bruteforce.h
        include/ufjfmltk/classifier/hnswlib/hnswalg.h
        include/ufjfmltk/classifier/hnswlib/hnswlib.h
        include/ufjfmltk/classifier/hnswlib/space_ip.h
        include/ufjfmltk/classifier/hnswlib/space_l2.h
        include/ufjfmltk/classifier/hnswlib/visited_list_pool.h
        )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../ufjfmltk-core/include")
add_library(${CLASSIFIER_TARGET} ${${CLASSIFIER_TARGET}_SRC} ${${CLASSIFIER_TARGET}_HDR})

add_library(${PROJECT_NAME}::${CLASSIFIER_TARGET} ALIAS ${CLASSIFIER_TARGET})

set_target_properties(${CLASSIFIER_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${CLASSIFIER_TARGET}_HDR}")

target_include_directories(${CLASSIFIER_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${CLASSIFIER_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/classifier" COMPONENT dev)

install(FILES include/ufjfmltk/Classifier.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CLASSIFIER_TARGET} for export ${PROJECT_NAME}.")
