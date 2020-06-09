add_library(${LIBVALIDATION} SHARED
    src/Validation.cpp
)

set_target_properties(${LIBVALIDATION} PROPERTIES PUBLIC_HEADER "include/Validation.hpp")

target_include_directories(${LIBVALIDATION} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBVALIDATION} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBVALIDATION} PUBLIC LIBVALIDATION_VERSION=1.0)
target_compile_features(${LIBVALIDATION} PRIVATE cxx_std_14)
target_link_libraries(${LIBVALIDATION} ${LIBCORE} ${LIBCLASSIFIER})

install_lib(${LIBVALIDATION} Validation)