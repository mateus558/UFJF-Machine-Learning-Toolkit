set(CLUSTERER_TARGET ${PROJECT_NAME}-clusterer)

# List of CPP (source) library files.
set(${CLUSTERER_TARGET}_SRC
        src/KMeans.cpp
        )

# List of HPP (header) library files.
set(${CLUSTERER_TARGET}_HDR
        include/ufjfmltk/clusterer/Clusterer.hpp
        include/ufjfmltk/clusterer/KMeans.hpp
        )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../ufjfmltk-core/include")
# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${CLUSTERER_TARGET} ${${CLUSTERER_TARGET}_SRC} ${${CLUSTERER_TARGET}_HDR})

add_library(${PROJECT_NAME}::${CLUSTERER_TARGET} ALIAS ${CLUSTERER_TARGET})

# Set two minimum target properties for the library.
# See https://cmake.org/cmake/help/latest/command/set_target_properties.html
# Properties are: 1) Library version number 2) list of corresponding public headers
set_target_properties(${CLUSTERER_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${CLUSTERER_TARGET}_HDR}")

# Specify include directories for both compilation and installation process.
# The $<INSTALL_PREFIX> generator expression is useful to ensure to create
# relocatable configuration files, see https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-relocatable-packages
target_include_directories(${CLUSTERER_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
install(TARGETS ${CLUSTERER_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/clusterer" COMPONENT dev)

install(FILES include/ufjfmltk/Clusterer.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CLUSTERER_TARGET} for export ${PROJECT_NAME}.")