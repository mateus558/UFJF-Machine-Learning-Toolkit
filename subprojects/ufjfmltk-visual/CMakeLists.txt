# This is the "physical" (i.e. real) name of the library.
# The actual file name of the library built is constructed based on conventions
# of the native platform, such as lib<LIBRARY_TARGET_NAME>.a,
# lib<LIBRARY_TARGET_NAME>.dylib or <LIBRARY_TARGET_NAME>.lib.
set(VISUAL_TARGET ${PROJECT_NAME}-visual)

# List of CPP (source) library files.
set(${VISUAL_TARGET}_SRC
        src/gnuplot_i.cpp
        src/Visualization.cpp
        )

# List of HPP (header) library files.
set(${VISUAL_TARGET}_HDR
        include/ufjfmltk/visual/gnuplot_i.hpp
        include/ufjfmltk/visual/Visualization.hpp
        include/ufjfmltk/Visualize.hpp
        )

# You can add an external dependency using the find_package() function call
# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${VISUAL_TARGET} ${${VISUAL_TARGET}_SRC} ${${VISUAL_TARGET}_HDR})
add_library(${PROJECT_NAME}::${VISUAL_TARGET} ALIAS ${VISUAL_TARGET})

# Set two minimum target properties for the library.
set_target_properties(${VISUAL_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${VISUAL_TARGET}_HDR}")

# Specify include directories for both compilation and installation process.
target_include_directories(${VISUAL_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

if(APPLE)
    target_link_libraries(${VISUAL_TARGET} ufjfmltk-core)
else()
    target_link_libraries(${VISUAL_TARGET} ufjfmltk-core stdc++fs)
endif(APPLE)
# Specify installation targets, typology and destination folders.
install(TARGETS ${VISUAL_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/visual" COMPONENT dev)

install(FILES include/ufjfmltk/Visualize.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${VISUAL_TARGET} for export ${PROJECT_NAME}.")