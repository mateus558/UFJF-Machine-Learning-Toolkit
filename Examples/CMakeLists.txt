set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(Core)
add_subdirectory(Classifier)
add_subdirectory(Ensemble)
add_subdirectory(Regression)
add_subdirectory(Validation)
add_subdirectory(Clustering)
add_subdirectory(FeatureSelection)