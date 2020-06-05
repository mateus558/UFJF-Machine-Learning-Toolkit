add_library(${LIBCLASSIFIER} SHARED
    src/Classifier.hpp
    src/DualClassifier.hpp
    src/PrimalClassifier.hpp
    src/IMA.hpp
    src/IMA.cpp
    src/KNN.hpp
    src/KNN.cpp
    src/OneVsAll.hpp
    src/OneVsAll.cpp
    src/Perceptron.hpp
    src/Perceptron.cpp
    src/SMO.hpp
    src/SMO.cpp
)

target_include_directories(${LIBCLASSIFIER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBCLASSIFIER} PUBLIC LIBCLASSIFIER_VERSION=1.0)
target_compile_features(${LIBCLASSIFIER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLASSIFIER} ${LIBCORE})