add_library(${LIBCLASSIFIER} SHARED
    src/IMA.cpp
    src/KNN.cpp
    src/Perceptron.cpp
    src/SMO.cpp
        include/OneVsAll.hpp
)

set_target_properties(${LIBCLASSIFIER} PROPERTIES PUBLIC_HEADER "include/Classifier.hpp;include/DualClassifier.hpp;include/PrimalClassifier.hpp;include/IMA.hpp;include/KNN.hpp;include/OneVsAll.hpp;include/Perceptron.hpp;include/SMO.hpp")

target_include_directories(${LIBCLASSIFIER} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBCLASSIFIER} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBCLASSIFIER} PUBLIC LIBCLASSIFIER_VERSION=1.0)
target_compile_features(${LIBCLASSIFIER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLASSIFIER} ${LIBCORE})

install_lib(${LIBCLASSIFIER} Classifier 1.0)