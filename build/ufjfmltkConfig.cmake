set(ufjfmltk_VERSION 0.51.8)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ufjfmltkConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################



if(NOT TARGET ufjfmltk::ufjfmltk)
  include("${CMAKE_CURRENT_LIST_DIR}/ufjfmltkTargets.cmake")
endif()

# Compatibility
get_property(ufjfmltk_ufjfmltk_INCLUDE_DIR TARGET ufjfmltk::ufjfmltk PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

set(ufjfmltk_LIBRARIES ufjfmltk::ufjfmltk)
set(ufjfmltk_INCLUDE_DIRS "${ufjfmltk_ufjfmltk_INCLUDE_DIR}")
list(REMOVE_DUPLICATES ufjfmltk_INCLUDE_DIRS)


