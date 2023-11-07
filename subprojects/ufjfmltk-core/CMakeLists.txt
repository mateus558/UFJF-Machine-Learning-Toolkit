set(CORE_TARGET core)

# List of CPP (source) library files.
set(${CORE_TARGET}_SRC
        src/Utils.cpp
        src/Datasets.cpp
        src/Random.cpp
        )

# List of HPP (header) library files.
set(${CORE_TARGET}_HDR
        include/ufjfmltk/core/Data.hpp
        include/ufjfmltk/core/Datasets.hpp
        include/ufjfmltk/core/Point.hpp
        include/ufjfmltk/core/DistanceMetric.hpp
        include/ufjfmltk/core/Kernel.hpp
        include/ufjfmltk/core/Learner.hpp
        include/ufjfmltk/core/Point.hpp
        include/ufjfmltk/core/Random.hpp
        include/ufjfmltk/core/Sampling.hpp
        include/ufjfmltk/core/Solution.hpp
        include/ufjfmltk/core/Statistics.hpp
        include/ufjfmltk/core/Timer.hpp
        include/ufjfmltk/core/Utils.hpp
        include/ufjfmltk/core/ExprOps.hpp
        include/ufjfmltk/core/ExprScalar.hpp
        include/ufjfmltk/core/ExprTraits.hpp
        include/ufjfmltk/core/ThreadPool.hpp
        )

# Adds a library target called ${LIBRARY_TARGET_NAME} to be built from the
# source and header files listed in the command invocation.
add_library(${CORE_TARGET} ${${CORE_TARGET}_SRC} ${${LIBRARY_TARGET_NAME}_HDR} src/Random.cpp)

add_library(${PROJECT_NAME}::${CORE_TARGET} ALIAS ${CORE_TARGET})

set_target_properties(${CORE_TARGET} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${CORE_TARGET}_HDR}")

target_include_directories(${CORE_TARGET} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
install(TARGETS ${CORE_TARGET}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/core" COMPONENT dev)

install(FILES include/ufjfmltk/Core.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${CORE_TARGET} for export ${PROJECT_NAME}.")