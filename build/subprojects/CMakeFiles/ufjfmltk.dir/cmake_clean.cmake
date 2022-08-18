file(REMOVE_RECURSE
  "../lib/libufjfmltk.a"
  "../lib/libufjfmltk.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ufjfmltk.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
