add_library(${LIBCLASSIFIER} SHARED
    src/IMA.cpp
    src/KNN.cpp
    src/OneVsAll.cpp
    src/Perceptron.cpp
    src/SMO.cpp
)

set_target_properties(${LIBCLASSIFIER} PROPERTIES PUBLIC_HEADERS "include/Classifier.hpp;include/DualClassifier.hpp;include/PrimalClassifier.hpp;include/IMA.hpp;include/KNN.hpp;include/OneVsAll.hpp;include/Perceptron.hpp;include/SMO.hpp")

target_include_directories(${LIBCLASSIFIER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${LIBCLASSIFIER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBCLASSIFIER} PUBLIC LIBCLASSIFIER_VERSION=1.0)
target_compile_features(${LIBCLASSIFIER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLASSIFIER} ${LIBCORE})

include(GNUInstallDirs)

install(TARGETS ${LIBCLASSIFIER}
        EXPORT ${PROJECT_NAME}_${LIBCLASSIFIER}-targets
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${LIBCLASSIFIER}
        )

install(EXPORT ${PROJECT_NAME}_${LIBCLASSIFIER}-targets DESTINATION ${CMAKE_INSTALL_DIR}/cmake/${LIBCLASSIFIER})