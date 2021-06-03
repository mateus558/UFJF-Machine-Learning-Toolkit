set(REGRESSOR_TARGET regressor)

# List of HPP (header) library files.
set(${REGRESSOR_TARGET}_HDR
        include/ufjfmltk/regressor/Regressor.hpp
        include/ufjfmltk/regressor/DualRegressor.hpp
        include/ufjfmltk/regressor/KNNRegressor.hpp
        include/ufjfmltk/regressor/LMS.hpp
        include/ufjfmltk/regressor/PrimalRegressor.hpp
        include/ufjfmltk/regressor/Metrics.hpp
        include/ufjfmltk/Regressor.hpp
        )

# You can add an external dependency using the find_package() function call
# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${REGRESSOR_TARGET} INTERFACE )
add_library(${PROJECT_NAME}::${REGRESSOR_TARGET} ALIAS ${REGRESSOR_TARGET})

target_link_libraries(${REGRESSOR_TARGET} INTERFACE ufjfmltk::core)

# Specify include directories for both compilation and installation process.
target_include_directories(${REGRESSOR_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
install(TARGETS ${REGRESSOR_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${REGRESSOR_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/regressor")

install(FILES include/ufjfmltk/Regressor.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${REGRESSOR_TARGET} for export ${PROJECT_NAME}.")