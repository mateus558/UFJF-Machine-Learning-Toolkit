set(CLASSIFIER_TARGET classifier)

# List of CPP (source) library files.
set(${CLASSIFIER_TARGET}_SRC
        src/int_dll.cpp
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
        include/ufjfmltk/classifier/int_dll.h
        include/ufjfmltk/classifier/SMO.hpp
        include/ufjfmltk/Classifier.hpp
        )

# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${CLASSIFIER_TARGET} ${${CLASSIFIER_TARGET}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

add_library(${PROJECT_NAME}::${CLASSIFIER_TARGET} ALIAS ${CLASSIFIER_TARGET})

target_link_libraries(${CLASSIFIER_TARGET} PRIVATE ufjfmltk::core)

set_target_properties(${CLASSIFIER_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${CLASSIFIER_TARGET}_HDR}")

target_include_directories(${CLASSIFIER_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
install(TARGETS ${CLASSIFIER_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/classifier" COMPONENT dev)

install(FILES include/ufjfmltk/Classifier.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CLASSIFIER_TARGET} for export ${PROJECT_NAME}.")