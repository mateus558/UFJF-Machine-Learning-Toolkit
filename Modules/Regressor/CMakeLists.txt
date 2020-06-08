add_library(${LIBREGRESSOR} SHARED
    src/LMS.cpp
    src/KNNRegressor.cpp
)

set_target_properties(${LIBREGRESSOR} PROPERTIES PUBLIC_HEADER
        "include/Regressor.hpp;include/PrimalRegressor.hpp;include/DualRegressor.hpp;include/LMS.hpp;include/KNNRegressor.hpp"
        )

target_include_directories(${LIBREGRESSOR} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(${LIBREGRESSOR} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)
target_compile_definitions(${LIBREGRESSOR} PUBLIC LIBREGRESSOR_VERSION=1.0)
target_compile_features(${LIBREGRESSOR} PRIVATE cxx_std_14)
target_link_libraries(${LIBREGRESSOR} ${LIBCORE})

install_lib(${LIBREGRESSOR})