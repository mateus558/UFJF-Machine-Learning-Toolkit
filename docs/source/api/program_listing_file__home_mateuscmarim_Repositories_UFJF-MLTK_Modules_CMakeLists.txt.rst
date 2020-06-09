
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_CMakeLists.txt:

Program Listing for File CMakeLists.txt
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_CMakeLists.txt>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/CMakeLists.txt``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   include(GNUInstallDirs)
   
   # Function to install a library in the system
   function(install_lib LIBTARGET INCLUDE_FOLDER)
       install(TARGETS ${LIBTARGET}
               EXPORT ${PROJECT_NAME}_${LIBTARGET}-targets
               LIBRARY DESTINATION
                   ${CMAKE_INSTALL_LIBDIR}
               RUNTIME DESTINATION
                   ${RUNTIME_DIR}
               PUBLIC_HEADER DESTINATION
                   ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${INCLUDE_FOLDER}
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
   
   # link all built libraries to the main library
   add_library(${LIBMAIN} SHARED ${OBJECTS})
   install_lib(${LIBMAIN} UFJF-MLTK)
