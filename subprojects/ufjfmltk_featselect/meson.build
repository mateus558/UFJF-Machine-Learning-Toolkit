project(
  'ufjfmltk_featselect',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK feature selection library.'

project_source_files = ['src/AOS.cpp', 'src/Fisher.cpp', 'src/Golub.cpp', 'src/RFE.cpp']
project_headers = ['include/AOS.hpp', 
    'include/FeatureSelection.hpp', 
    'include/Fisher.hpp', 
    'include/Golub.hpp',
    'include/RFE.hpp']

project_dependencies = [
  dependency('ufjfmltk_core', fallback : ['ufjfmltk_core', 'ufjfmltk_core_dep'], required : true),
  dependency('ufjfmltk_classifier', fallback : ['ufjfmltk_classifier', 'ufjfmltk_classifier_dep'], required : true),
  dependency('ufjfmltk_valid', fallback : ['ufjfmltk_valid', 'ufjfmltk_valid_dep'], required : true)
]

public_headers = include_directories('include')

install_headers('FeatureSelection.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : 'ufjfmltk/featselect')

project_target = static_library(
  meson.project_name(),
  project_source_files,
  dependencies: project_dependencies,
  install : true,
  include_directories : public_headers,
)

project_target_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_target_dep)
set_variable(meson.project_name() + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk/featselect'
)
