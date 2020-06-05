add_library(${LIBVALIDATION} SHARED
    src/Validation.hpp
    src/Validation.cpp
)

target_include_directories(${LIBVALIDATION} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBVALIDATION} PUBLIC LIBVALIDATION_VERSION=1.0)
target_compile_features(${LIBVALIDATION} PRIVATE cxx_std_14)
target_link_libraries(${LIBVALIDATION} ${LIBCORE} ${LIBCLASSIFIER})