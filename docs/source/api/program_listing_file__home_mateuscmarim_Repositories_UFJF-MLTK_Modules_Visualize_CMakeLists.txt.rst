
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_CMakeLists.txt:

Program Listing for File CMakeLists.txt
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_CMakeLists.txt>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Visualize/CMakeLists.txt``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   find_package(Gnuplot)
   
   if(GNUPLOT_FOUND)
       add_library(${LIBVISUALIZE} SHARED 
           src/gnuplot_i.cpp
           include/gnuplot_i.hpp
           src/Visualization.cpp
       )
   
       set_target_properties(${LIBVISUALIZE} PROPERTIES PUBLIC_HEADER "include/Visualization.hpp")
   
       target_include_directories(${LIBVISUALIZE} PUBLIC
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
               $<INSTALL_INTERFACE:include>)
       target_include_directories(${LIBVISUALIZE} PUBLIC
               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
               $<INSTALL_INTERFACE:src>)
       target_compile_definitions(${LIBVISUALIZE} PUBLIC LIBVISUALIZE_VERSION=1.1)
       target_compile_features(${LIBVISUALIZE} PRIVATE cxx_std_14)
       target_link_libraries(${LIBVISUALIZE} ${LIBCORE})
   
       install_lib(${LIBVISUALIZE} Visualize)
   
       set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${LIBVISUALIZE}> )
   else()
       message(WARNING "Gnuplot not found: The Visualization module will not be compiled.")
   endif()
