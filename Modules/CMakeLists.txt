add_subdirectory(Core)
add_subdirectory(Visualize)
add_subdirectory(Classifier)
add_subdirectory(Regressor)
add_subdirectory(Clusterer)
add_subdirectory(FeatureSelection)
add_subdirectory(Validation)

# function to generate the GUI executable in the future
# function(ufjf_mltk_gui teste)
#     foreach(arg IN LISTS ARGN)
#         message(STATUS "ufjf_mltk_gui test on arg1: ${arg}")
#     endforeach()
#     foreach(arg IN LISTS ARGV)
#         message(STATUS "ufjf_mltk_gui test on arg1: ${arg}")
#     endforeach()
# endfunction()

# ufjf_mltk_gui("teste" 1 2 3)

