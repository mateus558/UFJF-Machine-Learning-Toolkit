include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Function to install a library in the system
function(install_lib LIBTARGET INCLUDE_FOLDER LIBVERSION)
    if(CMAKE_BUILD_TYPE EQUAL "Debug")
        target_compile_definitions(${LIBTARGET} PUBLIC DEBUG=1)
    endif()
    if(UNIX)
        install(TARGETS ${LIBTARGET}
                EXPORT ${LIBTARGET}Targets
                LIBRARY DESTINATION
                    ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION
                    ${RUNTIME_DIR}
                PUBLIC_HEADER DESTINATION
                    ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${INCLUDE_FOLDER}/include
                )
    endif ()
    if(WIN32) 
        install(TARGETS ${LIBTARGET}
                EXPORT ${LIBTARGET}Targets
                LIBRARY DESTINATION
                    ${CMAKE_INSTALL_LIBDIR}
                RUNTIME DESTINATION
                    ${RUNTIME_DIR}
                PUBLIC_HEADER DESTINATION
                    ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${INCLUDE_FOLDER}
                )
    endif()

    write_basic_package_version_file(${LIBTARGET}ConfigVersion.cmake
            VERSION {${LIBVERSION}}
            COMPATIBILITY SameMajorVersion
            )
    export(TARGETS
            ${LIBTARGET}
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBTARGET}Config.cmake"
    )
    #install(EXPORT ${LIBTARGET}Config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBTARGET})
    install(EXPORT ${LIBTARGET}Targets FILE ${LIBTARGET}Targets.cmake DESTINATION  ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBTARGET})
    #install(FILES ${LIBTARGET}ConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBTARGET})
endfunction()

add_subdirectory(Core)
set(OBJECTS $<TARGET_OBJECTS:${LIBCORE}> )

if (BUILD_LIBVISUALIZE OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Visualize)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBVISUALIZE}> )
endif ()
if (BUILD_LIBCLASSIFIER OR BUILD_LIBVALIDATION OR BUILD_LIBFEATSELECT OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Classifier)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBCLASSIFIER}> )
endif ()
if (BUILD_LIBREGRESSOR OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Regressor)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBREGRESSOR}> )
endif ()
if (BUILD_LIBCLUSTERER OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Clusterer)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBCLUSTERER}> )
endif ()
if (BUILD_LIBFEATSELECT OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(FeatureSelection)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBFEATSELECT}> )
endif ()
if (BUILD_LIBVALIDATION OR BUILD_LIBFEATSELECT OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Validation)
    set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBVALIDATION}> )
endif ()

# link all built libraries to the main library
add_library(${LIBMAIN} SHARED ${OBJECTS})
install_lib(${LIBMAIN} UFJF-MLTK ${PROJECT_VERSION})