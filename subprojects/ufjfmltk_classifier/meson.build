project(
  'ufjfmltk_classifier',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_dependencies = [
]

core_dep = dependency('ufjfmltk_core', fallback : ['ufjfmltk_core', 'ufjfmltk_core_dep'])

project_dependencies += core_dep

build_args = []
project_description = 'UFJF-MLTK classifier library.'

project_source_files = ['src/IMA.cpp', 'src/Perceptron.cpp', 'src/SMO.cpp']
project_headers = ['include/Classifier.hpp', 
    'include/DualClassifier.hpp', 
    'include/IMA.hpp', 
    'include/KNNClassifier.hpp',
    'include/OneVsAll.hpp',
    'include/OneVsOne.hpp',
    'include/Perceptron.hpp',
    'include/PrimalClassifier.hpp',
    'include/SMO.hpp',
    'include/hnswlib/bruteforce.h',
    'include/hnswlib/hnswalg.h',
    'include/hnswlib/hnswlib.h',
    'include/hnswlib/space_ip.h',
    'include/hnswlib/space_l2.h',
    'include/hnswlib/visited_list_pool.h']

public_headers = include_directories('include')

build_args = [
]

install_headers('Classifier.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : 'ufjfmltk/classifier')

project_target = static_library(
  meson.project_name(),
  project_source_files,
  dependencies: core_dep,
  install : true,
  include_directories : public_headers,
)

project_target_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_target_dep)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk/classifier',
  libraries : project_target,
)
