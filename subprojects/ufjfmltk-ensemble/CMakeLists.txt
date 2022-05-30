set(ENSEMBLE_TARGET ensemble)

# List of HPP (header) library files.
set(${ENSEMBLE_TARGET}_HDR
        include/ufjfmltk/ensemble/AdaBoostClassifier.hpp
        include/ufjfmltk/ensemble/AutoWeightedVoting.hpp
        include/ufjfmltk/ensemble/BaggingClassifier.hpp
        include/ufjfmltk/ensemble/Ensemble.hpp
        include/ufjfmltk/ensemble/PerceptronCommittee.hpp
        include/ufjfmltk/ensemble/VotingClassifier.hpp
        )

# Adds a library target called ${ENSEMBLE_TARGET} to be built from the
# source and header files listed in the command invocation.
add_library(${ENSEMBLE_TARGET} INTERFACE)
add_library(${PROJECT_NAME}::${ENSEMBLE_TARGET} ALIAS ${ENSEMBLE_TARGET})

# Specify include directories for both compilation and installation process.
target_include_directories(${ENSEMBLE_TARGET} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${ENSEMBLE_TARGET} INTERFACE ufjfmltk::core ufjfmltk::classifier ufjfmltk::valid)

# Specify installation targets, typology and destination folders.
install(TARGETS ${ENSEMBLE_TARGET}
        EXPORT  ${PROJECT_NAME})

install(FILES ${${ENSEMBLE_TARGET}_HDR}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk/ensemble")

install(FILES include/ufjfmltk/Ensemble.hpp
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ufjfmltk")

message(STATUS "Created target ${ENSEMBLE_TARGET} for export ${PROJECT_NAME}.")