
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Clusterer_CMakeLists.txt:

Program Listing for File CMakeLists.txt
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Clusterer_CMakeLists.txt>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Clusterer/CMakeLists.txt``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   add_library(${LIBCLUSTERER} SHARED
       src/KMeans.cpp
   )
   
   set_target_properties(${LIBCLUSTERER} PROPERTIES PUBLIC_HEADER "include/Clusterer.hpp;include/KMeans.hpp")
   
   target_include_directories(${LIBCLUSTERER} PUBLIC
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>)
   target_include_directories(${LIBCLUSTERER} PUBLIC
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
           $<INSTALL_INTERFACE:src>)
   target_compile_definitions(${LIBCLUSTERER} PUBLIC LIBCLUSTERER_VERSION=1.0)
   target_compile_features(${LIBCLUSTERER} PRIVATE cxx_std_14)
   target_link_libraries(${LIBCLUSTERER} ${LIBCORE})
   
   install_lib(${LIBCLUSTERER} Clusterer)
