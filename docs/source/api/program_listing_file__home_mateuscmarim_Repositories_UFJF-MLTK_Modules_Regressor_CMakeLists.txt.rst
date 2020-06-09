
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_CMakeLists.txt:

Program Listing for File CMakeLists.txt
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_CMakeLists.txt>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Regressor/CMakeLists.txt``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   add_library(${LIBREGRESSOR} SHARED
       src/LMS.cpp
       src/KNNRegressor.cpp
   )
   
   set_target_properties(${LIBREGRESSOR} PROPERTIES PUBLIC_HEADER
           "include/Regressor.hpp;include/PrimalRegressor.hpp;include/DualRegressor.hpp;include/LMS.hpp;include/KNNRegressor.hpp"
           )
   
   target_include_directories(${LIBREGRESSOR} PUBLIC
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>)
   target_include_directories(${LIBREGRESSOR} PUBLIC
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
           $<INSTALL_INTERFACE:src>)
   target_compile_definitions(${LIBREGRESSOR} PUBLIC LIBREGRESSOR_VERSION=1.0)
   target_compile_features(${LIBREGRESSOR} PRIVATE cxx_std_14)
   target_link_libraries(${LIBREGRESSOR} ${LIBCORE})
   
   install_lib(${LIBREGRESSOR} Regressor)
