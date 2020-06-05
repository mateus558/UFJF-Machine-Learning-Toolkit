add_library(${LIBREGRESSOR} SHARED
    src/Regressor.hpp
    src/PrimalRegressor.hpp
    src/DualRegressor.hpp
    src/LMS.hpp
    src/LMS.cpp
    src/KNNRegressor.hpp
    src/KNNRegressor.cpp
)

target_include_directories(${LIBREGRESSOR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBREGRESSOR} PUBLIC LIBREGRESSOR_VERSION=1.0)
target_compile_features(${LIBREGRESSOR} PRIVATE cxx_std_14)
target_link_libraries(${LIBREGRESSOR} ${LIBCORE})