#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ufjfmltk::core" for configuration "Release"
set_property(TARGET ufjfmltk::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ufjfmltk::core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcore.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ufjfmltk::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_ufjfmltk::core "${_IMPORT_PREFIX}/lib/libcore.a" )

# Import target "ufjfmltk::classifier" for configuration "Release"
set_property(TARGET ufjfmltk::classifier APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ufjfmltk::classifier PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libclassifier.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ufjfmltk::classifier )
list(APPEND _IMPORT_CHECK_FILES_FOR_ufjfmltk::classifier "${_IMPORT_PREFIX}/lib/libclassifier.a" )

# Import target "ufjfmltk::visual" for configuration "Release"
set_property(TARGET ufjfmltk::visual APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ufjfmltk::visual PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvisual.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ufjfmltk::visual )
list(APPEND _IMPORT_CHECK_FILES_FOR_ufjfmltk::visual "${_IMPORT_PREFIX}/lib/libvisual.a" )

# Import target "ufjfmltk::ufjfmltk" for configuration "Release"
set_property(TARGET ufjfmltk::ufjfmltk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ufjfmltk::ufjfmltk PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libufjfmltk.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ufjfmltk::ufjfmltk )
list(APPEND _IMPORT_CHECK_FILES_FOR_ufjfmltk::ufjfmltk "${_IMPORT_PREFIX}/lib/libufjfmltk.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
