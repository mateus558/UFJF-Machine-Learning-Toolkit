function(install_lib LIBTARGET)
    include(GNUInstallDirs)

    install(TARGETS ${LIBTARGET}
            EXPORT ${PROJECT_NAME}_${LIBTARGET}-targets
            LIBRARY DESTINATION
                ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION
                ${RUNTIME_DIR}
            PUBLIC_HEADER DESTINATION
                ${CMAKE_INSTALL_INCLUDEDIR}/${LIBTARGET}
            )

    install(EXPORT ${PROJECT_NAME}_${LIBTARGET}-targets DESTINATION ${CMAKE_INSTALL_DIR}/cmake/${LIBTARGET})
endfunction()

add_subdirectory(Core)
set(OBJECTS $<TARGET_OBJECTS:${LIBCORE}> )

if (BUILD_LIBVISUALIZE OR BUILD_CLI OR BUILD_GUI)
    add_subdirectory(Visualize)
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

add_library(${LIBMAIN} SHARED 
    ${OBJECTS}
)

install_lib(${LIBMAIN})