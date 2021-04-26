set(REGRESSOR_TARGET ${PROJECT_NAME}-regressor)

# List of CPP (source) library files.
set(${REGRESSOR_TARGET}_SRC
        src/KNNRegressor.cpp
        src/LMS.cpp
        )

# List of HPP (header) library files.
set(${REGRESSOR_TARGET}_HDR
        include/ufjfmltk/regressor/Regressor.hpp
        include/ufjfmltk/regressor/DualRegressor.hpp
        include/ufjfmltk/regressor/KNNRegressor.hpp
        include/ufjfmltk/regressor/LMS.hpp
        include/ufjfmltk/regressor/PrimalRegressor.hpp
        )

# You can add an external dependency using the find_package() function call
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../ufjfmltk-core/include")
# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${REGRESSOR_TARGET} ${${REGRESSOR_TARGET}_SRC} ${${REGRESSOR_TARGET}_HDR})
add_library(${PROJECT_NAME}::${REGRESSOR_TARGET} ALIAS ${REGRESSOR_TARGET})

# Set two minimum target properties for the library.
set_target_properties(${REGRESSOR_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${REGRESSOR_TARGET}_HDR}")

# Specify include directories for both compilation and installation process.
target_include_directories(${REGRESSOR_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
install(TARGETS ${REGRESSOR_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/regressor" COMPONENT dev)

install(FILES include/ufjfmltk/Regressor.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${REGRESSOR_TARGET} for export ${PROJECT_NAME}.")
