add_library(${LIBREGRESSOR} SHARED
    src/LMS.cpp
    src/KNNRegressor.cpp
)

set_target_properties(${LIBREGRESSOR} PROPERTIES PUBLIC_HEADER
        "include/Regressor.hpp;include/PrimalRegressor.hpp;include/DualRegressor.hpp;include/LMS.hpp;include/KNNRegressor.hpp"
        )

target_include_directories(${LIBREGRESSOR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${LIBREGRESSOR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${LIBREGRESSOR} PUBLIC LIBREGRESSOR_VERSION=1.0)
target_compile_features(${LIBREGRESSOR} PRIVATE cxx_std_14)
target_link_libraries(${LIBREGRESSOR} ${LIBCORE})

include(GNUInstallDirs)

install(TARGETS ${LIBREGRESSOR}
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${LIBREGRESSOR}
        )