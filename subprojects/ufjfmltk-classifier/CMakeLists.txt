set(CLASSIFIER_TARGET ${PROJECT_NAME}-classifier)

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
        include/ufjfmltk/Classifier.hpp
        )

add_library(${CLASSIFIER_TARGET} INTERFACE)
add_library(${PROJECT_NAME}::${CLASSIFIER_TARGET} ALIAS ${CLASSIFIER_TARGET})

# Specify include directories for both compilation and installation process.
target_include_directories(${CLASSIFIER_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${CLASSIFIER_TARGET} INTERFACE ufjfmltk-core)

# Specify installation targets, typology and destination folders.
install(TARGETS ${CLASSIFIER_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${CLASSIFIER_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/classifier")

install(FILES include/ufjfmltk/Classifier.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CLASSIFIER_TARGET} for export ${PROJECT_NAME}.")