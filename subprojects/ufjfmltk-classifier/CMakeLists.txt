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
