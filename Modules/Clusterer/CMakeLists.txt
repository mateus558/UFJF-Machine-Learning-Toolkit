add_library(${LIBCLUSTERER} SHARED
    src/Clusterer.hpp
    src/KMeans.hpp
    src/KMeans.cpp
)

target_include_directories(${LIBCLUSTERER} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBCLUSTERER} PUBLIC LIBCLUSTERER_VERSION=1.0)
target_compile_features(${LIBCLUSTERER} PRIVATE cxx_std_14)
target_link_libraries(${LIBCLUSTERER} ${LIBCORE})